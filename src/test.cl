
#ifdef TESTER
#define VAL 4
#else
#define VAL 3
#endif
void __kernel test(__global int* twoints){
    printf("Hello!\n");
    twoints[0] = VAL;
    twoints[1] = VAL*2;
    printf("(%d, %d)\n", twoints[0], twoints[1]);
}

void __kernel test2(__global int* twoints){
    printf("Hello!\n");
    twoints[0] = VAL*2;
    twoints[1] = VAL*4;
    printf("(%d, %d)\n", twoints[0], twoints[1]);
}