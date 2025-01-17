
#ifdef TESTER
#define VAL 4
#else
#define VAL 3
#endif
#define LOG(thing) printf("\x1b[33mKernel::Log\t" thing);printf( "\x1b[0m\n")
void __kernel test(__global int* twoints){
    printf("Hello!\n");
    twoints[0] = VAL;
    twoints[1] = VAL*2;
    printf("(%d, %d)\n", twoints[0], twoints[1]);
}

void __kernel test2(int a, int b){
    printf("%d + %d = %d\n", a, b, a+b);
}

//dot a 4x4 vector
void __kernel multiply(__global float* vecA, __global float* vecB, __global float* result){
    int index = get_global_id(0);
    result[index] = vecA[index] * vecB[index];
    
    printf("WorkItem: %d -- %f * %f = %f\n", index, vecA[index], vecB[index], result[index]);


}

__kernel void localMem(__local float* x){
    printf("Sup");
    float4 a = (float4) (1.0);
    printf("xyzw %f %f %f %f", a.x, a.s1, a.z, a.w);
    x[0] = a.x;

}
