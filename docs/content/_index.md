+++
title = "Debugging Workshop for Mobileye"
+++
  
# About Me

Hi! I'm Alex Kushnir, and I will be guiding you through this GDB workshop. This page will help you to get ready for our time together and will save us time to talk about really important things!
Let's get started!

---

# Workshop Syllabus

Day 1
-----
- gdb basics - command line
- gdb TUI
- Debug symbols 
- gdb in IDE
- Advanced Breakpoints

Day 2
-----
- Core dump analysis
- Attaching to a running process
- Altering execution
- Checkpoints 
- Tracepoints
- Reverse execution 
- Remote debugging
- Memory issues

Day 3
-----
- Multithreading
- Customizing gdb
- Complementary tools
- What's new in C++ 

---

# Setup the environment

I prepared a Docker image to support the workshop (this way we'll be working in an isolated environment, and it won't matter the OS you use)

# Support code for the Workshop

All code is stored in github repository [`https://github.com/alexkushnir/gdb_workshop`](https://github.com/alexkushnir/gdb_workshop)

# Docker Image Installation

To get started quickly, use a pre-configured docker image in one of 2 ways:

- Build the image locally using the Dockerfile from the github repo - [`https://github.com/alexkushnir/gdb_workshop/blob/master/Dockerfile`](https://github.com/alexkushnir/gdb_workshop/blob/master/Dockerfile)
	- `docker build -t gdb-workshop . ` (Assuming running from the same folder where the Dockerfile is)
	- `docker run -it --rm -v <local-dir>:/mnt gdb-workshop`
- Pull the image from Dockerhub
	- `docker pull alexkushnir1/gdb-workshop:latest`
	- `docker run -it --rm -v <local-dir>:/mnt alexkushnir1/gdb-workshop:latest`

This image contains all necessary tools pre-installed.

---

