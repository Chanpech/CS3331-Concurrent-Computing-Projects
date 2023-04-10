// Process the input arguments

//New allocating space for the point counts
//Access is regular indexing: printCounts[x][y]
//By my assumption this would need to be created first before fork/exec child.
int pixels, i;
pointCounts=(int**)malloc(pixels * sizeof(int*));
if(pointCounts==NULL){
    printf("Can malloc space for point coutns\n");
    exit(1);
}
for(i=0; i< pixels; i++){
    pointCounts[i] = (int *)malloc(pixels*sizeof(int));
    if(pointCoutns[i] == NULL){
        printf("Can't malloc space for point counts\n");
        exit(2);
    }
}
//End pointCounts allocation