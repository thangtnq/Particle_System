Given a bloated game particles system, I optimized it torun 4.41 time faster than the original one. What I did was:
1. SIMD instruction
2. Replace std Vector with customized linked list
3. Change user defined data alignment to reduce its size
4. Change primitive data types to better fit the program
5. Move methods that only need to be called once outside of loop.