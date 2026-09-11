#pragma once
// app/cli/version.h
// Declarations — filled in by the cc_versioned_binary genrule at build time.
namespace version {
const char* GetVersion();
const char* GetBuildLabel();
}  // namespace version
