# Changelog

All notable changes to this project will be documented in this file.

## [1.0.3] - 2025-09-25
- Simplified write/read registers functions to generic Single and Double functions
- Added Check functions for write/read registers functions
- Added error code during init if communication error with MCP23S17
- Replaced inline macros of registers to enum
- Changed name of attribute value -> ports in struct MCP23S17_t
- Replaced CalculateModuleId and CalculatePinId functions to inline macros

## [1.0.2] - 2025-09-23
- Fixed bug in REG_OLAT_B value

## [1.0.1] - 2025-08-22
- Added NULL check of cs functions

## [1.0.0] - 2025-04-23
- Added doxygen documentation for plib_mcp23s17_spi.h and plib_mcp23s17.h
- Replaced configure functions by write functions (during init)
- Renamed writePin by updatePin