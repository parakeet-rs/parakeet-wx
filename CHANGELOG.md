# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.0] - 2023-02-14

### Added

- Added description fields for all configuration options.

### Changed

- Don't use `wxLocale` with smart pointers to avoid crash on exit.
- (dev) Moved away from `RapidJSON` in favour of `nlohmann/json`.
- Upgrade `libparakeet` to v0.4.0, rewrite the decryption logic.

## [0.2.2] - 2022-11-23

### Changed

- Upgrade `libparakeet` to v0.2.2.
- Integrate new Ximalaya Scramble-Table-Generation API.
- (dev) Fixed Linux build produced by GitHub Actions.

## [0.2.1] - 2022-11-14

### Changed

- Upgrade `libparakeet` to v0.2.1.

## [0.2.0] - 2022-11-05

### Changed

- Re-implement the support for Kugou `kgm` / `vpr` format.

## Removed

- **BREAKING**: Removed support for Kugou using tables.

## [0.1.5] - 2022-11-05

### Added

- New project icon!

## [0.1.4] - 2022-11-04

### Changed

- Fixed audio type identification for `mp4` files.

## [0.1.3] - 2022-10-23

### Added

- Basic support for HiDPI screen.
- Support EncV2 variant of QMCv2 files.

## [0.1.2] - 2022-05-23

Maintenance release, new user facing features added or fixed.

## [0.1.0] - 2022-05-18

Initial release of this project.

### Added

- Support for Kugou `kgm` / `vpr` files.
- Support for KuWo `kwm` files.
- Support for Netease `ncm` files.
- Support for QMCv1 and partial support for QMCv2 files.
- Support for Xiami `xm` files.
- Support for Joox `ofl_en` files.
- Support for Ximalaya `x2m` / `x3m` files.

[0.1.0]: https://github.com/parakeet-rs/parakeet-wx/commits/v0.1.0-beta
[0.1.2]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.1.0-beta...v0.1.2-beta
[0.1.3]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.1.2-beta...v0.1.3
[0.1.4]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.1.3...v0.1.4
[0.1.5]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.1.4...v0.1.5
[0.2.0]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.1.5...v0.2.0
[0.2.1]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.2.0...v0.2.1
[0.2.2]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.2.1...v0.2.2
[0.3.0]: https://github.com/parakeet-rs/parakeet-wx/compare/v0.2.2...v0.3.0
