#!/bin/bash
# ---------------------------------------------------------------------------
# alioth (POCO F3 / Redmi K40) - HyperOS 1.0.5.0 stock-identity build wrapper
#
# Pins the build metadata that ends up inside UTS_VERSION, /proc/version and
# uname():
#   release : 4.19.157-perf-g92c089fc2d37
#   version : #1 SMP PREEMPT Tue Jan 13 12:57:01 UTC 2026
#   builder : builder@pangu-build-component-vendor-432215-vj7fq-j7z14-wt7cv
#
# NOTE: build/build.sh and prebuilts-master/ are NOT shipped in this repo.
# Either point BUILD_CONFIG at a checked-out build/ tree, or use the plain
# `make` invocation documented in the technical report.
# ---------------------------------------------------------------------------

export KBUILD_BUILD_USER="builder"
export KBUILD_BUILD_HOST="pangu-build-component-vendor-432215-vj7fq-j7z14-wt7cv"
export KBUILD_BUILD_TIMESTAMP="Tue Jan 13 12:57:01 UTC 2026"
export KBUILD_BUILD_VERSION=1

# Keep the SCM suffix out of the release string so it stays exactly
# 4.19.157-perf-g92c089fc2d37 (no trailing "-dirty" / commit hash).
echo "" > .scmversion

BUILD_BOOT_IMG=true BUILD_CONFIG=private/msm-google/build.config.alioth.no-cfi build/build.sh "$@"
