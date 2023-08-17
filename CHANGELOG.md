<!-- Copyright (c) 2023 Golioth, Inc. -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2023-08-17

### Breaking Changes

- This repo is now a Zephyr module. It should be included as a modules/lib
  subdirectory (e.g. modules/lib/libostentus) using west.yml. Add
  `CONFIG_LIB_OSTENTUS=y` to your configuration to add the library to the
  build.

### Added

- Changelog
- zephyr/module.yml; include this library at modules/lib/libostentus using
  west.yml
