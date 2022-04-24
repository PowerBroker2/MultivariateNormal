#include "MultivariateNormal.h"




void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Vector2f mean;
  mean << 0,
          0;
          
  Matrix2f covar;
  covar << 10, 0,
            0, 1;
  
  EigenMultivariateNormal<float> solver(mean, covar);

  while(true)
  {
    auto samples = solver.samples(5).transpose();
    Serial.print(samples(0, 0)); Serial.print(", "); Serial.println(samples(0, 1));
    Serial.print(samples(1, 0)); Serial.print(", "); Serial.println(samples(1, 1));
    Serial.print(samples(2, 0)); Serial.print(", "); Serial.println(samples(2, 1));
    Serial.print(samples(3, 0)); Serial.print(", "); Serial.println(samples(3, 1));
    Serial.print(samples(4, 0)); Serial.print(", "); Serial.println(samples(4, 1));
    Serial.println();
    delay(100);
  }
}




void loop()
{
  
}
