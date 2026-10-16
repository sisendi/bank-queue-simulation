# Bank Queue Simulation

# Project Overview

This project simulates a single-server bank queue using C++. Customers arrive at different times and wait for service when the bank server is busy.

# Objectives

* Simulate 100 customers arriving at a bank.
* Generate inter-arrival times uniformly between 1 and 5 minutes.
* Generate service times uniformly between 1 and 6 minutes.
* Calculate customer waiting times and time spent in the system.
* Analyse the overall performance of the queue.

## Simulation Parameters

* Number of customers: 100
* Inter-arrival time: 1–5 minutes
* Service time: 1–6 minutes
* Random seed used: 42

## Results

* Total simulation time: 354.50 minutes
* Average inter-arrival time: 2.92 minutes
* Average service time: 3.55 minutes
* Average waiting time: 30.44 minutes
* Average time in the system: 33.99 minutes

## Technologies Used

* C++
* Visual Studio Code
* GCC compiler

## Files

* `bank_queue_simulation.cpp` — simulation source code.
* `Bank_Queue_Simulation_Report.pdf` — project report.

## How to Run

Compile the C++ source code using GCC and run the resulting executable. Follow the program's prompts to enter the random seed if requested.