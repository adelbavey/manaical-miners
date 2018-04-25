int pow(int a, int b){
	int i;
	int res = 1;
	for(i = 0; i < b; i++){
		res = res * a;
	}
	return res;
}

long randomLong(long seed) {
  seed ^= (seed << 21);
  seed ^= (seed >> 35);
  seed ^= (seed << 4);
  return seed;
}

float cos(float x){
    int i,fact=1;
    float sum,term;
    sum=1.0;
    term=1.0;
    for(i=1;i<5;i++){
      term=term*x*x;
      term=term*(-1);
      fact=factorial(2*i);
      sum=sum+(term/fact);
    }
	return(sum);    
}

int factorial(int x){
    int fact;
    if(x==1)
        return(1);
    else
        fact=x*factorial(x-1);       
    return(fact);                
}

double abs(double i){
    return (i < 0 ? -i : i);
}

double floor(double x){
	if(x>0)return (int)x;
	return (int)(x-0.9999999999999999);
}

int floorInt(double x){
  int i = (int) x;
  if (i > x)
    i--;
  return i;
}