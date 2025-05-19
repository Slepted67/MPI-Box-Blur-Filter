# MPI Box Blur Filter

This project implements a parallel box blur filter using MPI. The program reads a high-resolution image, applies a blur using a configurable kernel size, and outputs the blurred result. Each process is assigned a portion of the image to process, demonstrating strong scaling and communication between MPI ranks.

## 🧠 Features
- Accepts any image input (PGM or raw RGB with known dimensions)
- Adjustable blur kernel size
- Strong scaling benchmarks
- Communication time vs computation time analysis

## 🛠 Technologies Used
- C (with MPI)
- OpenMPI
- PBS for batch job scheduling (tested on a cluster)
- Python (for graphing runtime results)

## 📈 What I Learned
- How to distribute image data across MPI processes
- Measuring strong scaling and communication overhead
- Writing batch scripts for job submission in an HPC environment

