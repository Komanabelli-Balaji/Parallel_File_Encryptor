# Crypter Project - Multiprocessing Branch

## Overview
This project is a C++ implementation of a file encryption and decryption utility. It is designed to handle file operations securely and efficiently, with a focus on modularity and maintainability. In the current branch, multiprocessing techniques have been integrated to improve performance, especially for large files and batch operations.

### Key Features
- **Encryption & Decryption:** Supports basic cryptographic operations on files.
- **File Handling:** Robust file I/O management for reading and writing files.
- **Multiprocessing:** Utilizes multiple processes to parallelize encryption/decryption tasks, reducing execution time for large datasets.
- **Process Management:** Modular process management for handling concurrent operations.

## Limitations
- **Platform Dependency:** Multiprocessing implementation may be platform-specific and could require adjustments for non-Windows systems.
- **Cryptography:** Uses basic encryption algorithms; not suitable for high-security requirements.
- **Error Handling:** Limited error handling for process failures and file access issues.
- **Scalability:** Multiprocessing is currently optimized for local execution; distributed processing is not supported.
- **Resource Usage:** High resource consumption for very large files or many concurrent processes.

## Future Developments
- **Advanced Cryptography:** Integrate stronger encryption algorithms and support for key management.
- **Cross-Platform Support:** Enhance compatibility with Linux and macOS.
- **Distributed Processing:** Extend multiprocessing to support distributed systems and cloud environments.
- **Improved Error Handling:** Add comprehensive error reporting and recovery mechanisms.
- **User Interface:** Develop a graphical or web-based interface for easier usage.
- **Configuration Options:** Allow users to customize process count, buffer sizes, and encryption parameters.

---
For more details, see the source files in the `src/` directory. Contributions and suggestions are welcome!