//Child set up shared memory segment
  z=0;
  size=0;
  increment=range/((double)pixels-1);  //-- How far we move at each step
  for (pheight=0;pheight<pixels;pheight++){
      c=start-(double)pheight*increment*I;
      for (pwidth=0;pwidth<pixels;pwidth++){
           z=0;
	       iterations=0;
           for (i=1;i<=maxiterations;i++){
                z=cpow(z,2)+c;
	            size=cabs(z);
	            if (size>2.0){
	                 iterations=i;
	                 break;
	            }
           }
	   

	      if (i>maxiterations)iterations=maxiterations;
	      //-- NEW
	      pointCounts[pheight][pwidth]=iterations; //-- Filling in pointCounts
	                                                //-- In the previous project just computed color
	                                                //-- for the point and addded point to PPM file
               c=c+increment+0*I;
      }      
  }
