# CryptoNote
A Visual Studio (Windows) C console application program to evaluate CryptoNote performance.

![![Chart](Data.png?raw=true "CryptoNote Performance on Various CPUs")
](Data.png?raw=true "CryptoNote Performance on Various CPUs")

## Motivation

Moving toward one CPU one Vote. This program collects performance information which will inform this effort.

## Overview

CrytoNote is designed to run efficiently on CPUs which have several MB of low latency memory available. AES operations are carried out on a 2MB buffer.

The presumption is that CPUs have enough available low latency memory to run CryptoNote without memory latency bottlenecks with a 2 MB buffer.

However, using this program, I have found that memory latency becomes a bottleneck for CPUs for a CryptoNote buffer size somewhere between 128KB and 256KB

## Long term approach

In order to have one CPU (core) one vote, we need a POW hash algorithm which requires most of a full block time to compute one hash for a typical CPU. 

The goal is to make it very hard for any ASIC designer to create a circuit which will have any significant cost/performance benefit over a generic CPU.

Note that AES hardware support apparently doubles the hash rate for CryptoNote. 

By choosing an appropriate memory size and number of iterations for CrytoNote, it is possible to target only CPUs with AES hardware support and low latency memory. No other device would be able to complete a single hash within the block time.

For verification, intermediate hash data may be saved to the blockchain and used by a highly parallel (GPU) algorithm to verify each block efficiently.

## Command line

CryptoNite [disable_AES_HW]


## Example output

AES H/W support is present:	FALSE

N hashes:	20

Memory(KB)	AVG ms/hash	STD

16384	256.0	5.03

8192	138.0	5.23

4096	82.0	4.10

2048	56.5	4.89

1024	42.9	5.00

512	34.0	5.03

256	24.0	5.03

128	22.0	4.10

64	20.5	2.24

32	19.5	2.24

16	19.0	3.08

8	19.0	3.08

4	18.5	3.66

2	19.1	2.95

# Windows x64 binary

[ZIP with Binary](CryptoNite.zip)
