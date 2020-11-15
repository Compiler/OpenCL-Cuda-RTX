void __kernel test(__global int* twoints){
    printf("Hello!\n");
    if(TESTER != 4){
        twoints[0] = 3;
        twoints[1] = 9;
    }else{
        twoints[0] = 4;
        twoints[1] = 8;
    }
    printf("(%d, %d)\n", twoints[0], twoints[1]);
}