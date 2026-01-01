# Crypter Project - Multithreading Branch (main branch)

## Overview
This branch of the Crypter project implements file encryption and decryption using multithreading in C++. The goal is to enhance performance by parallelizing tasks within a single process, making the application more responsive and efficient for large files and batch operations.

### Key Features
- **Encryption & Decryption:** Provides basic cryptographic operations for files.
- **File Handling:** Manages file I/O for reading and writing securely.
- **Multithreading:** Uses C++ threads to parallelize encryption/decryption, improving speed and resource utilization.
- **Thread Management:** Modular design for creating, managing, and synchronizing threads.

## Limitations
- **Thread Safety:** Some components may not be fully thread-safe, risking race conditions or data corruption.
- **Cryptography:** Relies on simple encryption algorithms; not suitable for high-security needs.
- **Scalability:** Performance gains depend on hardware; limited by the number of CPU cores.
- **Error Handling:** Basic error handling for thread failures and file access issues.
- **Platform Dependency:** Threading implementation may require adjustments for different operating systems.

## Future Developments
- **Advanced Cryptography:** Add support for stronger encryption algorithms and key management.
- **Improved Thread Safety:** Refactor code to ensure all operations are thread-safe.
- **Cross-Platform Support:** Enhance compatibility with Linux and macOS threading models.
- **Dynamic Thread Pool:** Implement a configurable thread pool for better resource management.
- **User Interface:** Develop a GUI or web interface for easier usage.
- **Distributed Processing:** Explore hybrid models combining multithreading and multiprocessing for further scalability.

---
For more details, see the source files in the `src/` directory. Contributions and suggestions are welcome!
