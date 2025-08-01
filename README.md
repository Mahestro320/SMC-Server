# SMC (Super Mega Cool) Server v0.2.2 by Mahestro_320

The SMC server (SMCS) is a server that receives TCP requests from an SMC client to access files, display directory contents, etc.

## How to compile

The client is programmed in **C++ (ISO C++20 Standard)**. To compile it, you can use **Visual Studio** or the **basic C++ compiler**

> [!WARNING]
> It needs **Boost 1.88.0+ to work**. To do this, **put the Boost include folder in `external` directory at root**

## How to use

### STRG (storage) directory

The STRG directory contains all server storage and is defined in the STRG constant of the C++ header file "include/directories.hpp"
At the root of STRG, there are two folders: network and users (their use is explained below)

### network directory

This folder contains all the users' personal folders with the following name format: "perso.<username>" and the shared folder "shared"

### users directory

This folder contains all the user folders, and each folder contains a file named ".uif" (UIF = User Informations) which has some information about the users (more details about the UIF file below)

### UIF file

A UIF (User Informations) file contains a 16-byte header consisting of a 4-byte signature: 55 49 46 00 ("UIF\x0"), then a single byte for the user's role (more information on roles below), and finally 11 bytes with a value of 0x00 to fill in the empty space. After the header comes 512 bytes for the user's name and password (255 bytes for the username and 255 for the password)

## User roles

There are three roles for users: User (basic user), Admin and Developer

### Basic user permissions

It can access its own directory and the shared directory in the network folder

### Admin permissions

It can access everything in the STRG folder

### Developer permissions

It can access all files on every drive connected to the server

## Welcome message

When a client connects, the server sends a welcome message: "SMC"

## Requests

Requests can be sent from the SMC client
IDs are defined in the C++ header file "include/network/request/id.hpp"

## Configuration file

Its location is `data\config.ini` and it contains this values below

> [!NOTE]
> each value is for the example

```ini
[server]

; The port on which the server accepts connections
port=5000

; If the paths given in this category do not have a drive
; letter, then they will be relative to the root
[dirs]

; Server storage
storage=srvdir

; Server logs
logs=logs
```

## Abbreviated names

- PUD: Protected User Data
- UPC: User Personnal Content
