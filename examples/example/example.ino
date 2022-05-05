#include "MultivariateNormal.h"




void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Vector2d mean;
  mean << 0,
          0;
          
  Matrix2d covar;
  covar << 10, 0,
            0, 1;
  
  EigenMultivariateNormal<double> multVNorm(mean, covar);

  while(true)
  {
    Serial.println("--------------------------------------------------");
    
    Matrix<double, 5, 2> samples = multVNorm.samples(5).transpose();
    Serial.println("5 random samples drawn from the distribution:");
    Serial.print(samples(0, 0)); Serial.print(", "); Serial.println(samples(0, 1));
    Serial.print(samples(1, 0)); Serial.print(", "); Serial.println(samples(1, 1));
    Serial.print(samples(2, 0)); Serial.print(", "); Serial.println(samples(2, 1));
    Serial.print(samples(3, 0)); Serial.print(", "); Serial.println(samples(3, 1));
    Serial.print(samples(4, 0)); Serial.print(", "); Serial.println(samples(4, 1));
    Serial.println();

    double evalMean = multVNorm.eval(mean);
    Serial.println("Distribution evaluated at the mean location:");
    Serial.println(evalMean);
    Serial.println();
    
    delay(1000);
  }
}




void loop()
{
  
}
