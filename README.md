# CTA_Alristrol_Gallardo 2.0

MIT License

Copyright (c) 2025 Alristrol Technologies Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 

## Overview

CTA_Alristrol_Gallardo 2.0 is a high-performance market data and trading server.  
It supports both receiving real-time quotes and executing trades across multiple exchanges.

What's new in 2.0:  
- Upgraded to C++17, leveraging modern language features.  
- Refactored architecture to reduce redundant code and improve modularity.  
- Supports trade execution in addition to market data ingestion.  
- Optimized multi-threading and memory management for ultra-low latency.  

## Key Features

- Real-Time Market Data: Fetch quotes, order books, and trades via WebSocket/REST APIs.  
- Trading Execution: Market/limit orders, leveraged positions, and order management.  
- Multi-Exchange Support: Unified interface for Bitmex, Binance, Bybit.  
- Low Latency: Optimized with multi-threading and memory management.  
- Modular Design: Easily extendable with new exchanges or strategies.  
- Logging & Monitoring: Comprehensive logging and live monitoring tools.  

## Version Comparison

| Feature / Version         | CTA_Alristrol_Gallardo 1.0 | CTA_Alristrol_Gallardo 2.0 |
|---------------------------|---------------------------|----------------------------|
| C++ Standard              | C++11                     | C++17                      |
| Architecture              | Original, more verbose    | Refactored, modular, cleaner|
| Market Data               | Yes                       | Yes                        |
| Trade Execution           | No                        | Yes                        |
| Code Redundancy           | Higher                    | Reduced                    |
| Multi-Threading           | Basic                     | Optimized                  |
| Memory Management         | Standard                  | Improved for low latency   |

## Prerequisites

- C++ Standard: C++17  
- Compiler: GCC 11+ or Clang 14+ (must support C++17)  
- CMake: 3.20+  
- Boost: For networking and threading utilities  

