# Components Directory

This directory is for custom components that can be reused across different ESP32S3 projects.

## Structure
Each component should have its own subdirectory with:
- `CMakeLists.txt` - Component build configuration
- `include/` - Public header files
- Source files (`.c`, `.cpp`)

## Example Component Structure
```
components/
└── my_component/
    ├── CMakeLists.txt
    ├── include/
    │   └── my_component.h
    └── my_component.c
```