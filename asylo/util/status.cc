/*
 *
 * Copyright 2017 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "asylo/util/status.h"

#include "absl/strings/str_cat.h"
#include "asylo/util/status_error_space.h"

namespace asylo {

Status::Status()
    : error_space_(
          error::error_enum_traits<error::GoogleError>::get_error_space()),
      error_code_(error::GoogleError::OK) {}

Status::Status(const error::ErrorSpace *space, int code, std::string message)
    : error_space_(space), error_code_(code), message_(std::move(message)) {}

Status Status::OkStatus() { return Status(); }

int Status::error_code() const { return error_code_; }

absl::string_view Status::error_message() const { return message_; }

const error::ErrorSpace *Status::error_space() const { return error_space_; }

bool Status::ok() const { return error_code_ == 0; }

std::string Status::ToString() const {
  return ok() ? error_space_->String(error_code_)
              : absl::StrCat(error_space_->SpaceName(),
                             "::", error_space_->String(error_code_), ": ",
                             message_);
}

Status Status::ToCanonical() const {
  // Allow at most one layer of error-code translation.
  return IsCanonical() ? *this : Status(CanonicalCode(), ToString());
}

error::GoogleError Status::CanonicalCode() const {
  return error_space_->GoogleErrorCode(error_code_);
}

void Status::SaveTo(StatusProto *status_proto) const {
  status_proto->set_code(error_code_);
  status_proto->set_error_message(message_);
  status_proto->set_space(error_space_->SpaceName());
  status_proto->set_canonical_code(CanonicalCode());
}

void Status::RestoreFrom(const StatusProto &status_proto) {
  // Set the error code from the error space, if recognized.
  error_space_ = error::ErrorSpace::Find(status_proto.space());
  if (error_space_) {
    // The canonical code must match the canonical code as computed by the
    // error space.
    if (status_proto.has_canonical_code() &&
        (error_space_->GoogleErrorCode(status_proto.code()) !=
         status_proto.canonical_code())) {
      SetInvalid();
      return;
    } else {
      error_code_ = status_proto.code();
    }
  } else {
    // Error space lookup failed. Use the canonical error space.
    error_space_ =
        error::error_enum_traits<error::GoogleError>::get_error_space();

    // Both error code and canonical code must be OK, or neither.
    if (status_proto.has_canonical_code() &&
        ((status_proto.code() == 0) != (status_proto.canonical_code() == 0))) {
      SetInvalid();
      return;
    }
    if (status_proto.has_canonical_code()) {
      error_code_ = status_proto.canonical_code();
    } else {
      // Default to error::GoogleError::UNKNOWN.
      error_code_ = error::GoogleError::UNKNOWN;
    }
  }
  if (!ok()) {
    message_ = status_proto.error_message();
  } else {
    message_.clear();
  }
}

bool Status::IsCanonical() const {
  return error_space_->SpaceName() == error::kCanonicalErrorSpaceName;
}

void Status::SetInvalid() {
  Set(error::StatusError::INVALID, "Failed to parse invalid StatusProto");
}

bool operator==(const Status &lhs, const Status &rhs) {
  return (lhs.error_code() == rhs.error_code()) &&
         (lhs.error_message() == rhs.error_message()) &&
         (lhs.error_space() == rhs.error_space());
}

bool operator!=(const Status &lhs, const Status &rhs) { return !(lhs == rhs); }

std::ostream &operator<<(std::ostream &os, const Status &status) {
  return os << status.ToString();
}

}  // namespace asylo
