# MultivariateNormal
[![GitHub version](https://badge.fury.io/gh/PowerBroker2%2FMultivariateNormal.svg)](https://badge.fury.io/gh/PowerBroker2%2FMultivariateNormal) [![arduino-library-badge](https://www.ardu-badge.com/badge/MultivariateNormal.svg?)](https://www.ardu-badge.com/MultivariateNormal)<br /><br />
Arduino library for sampling from a multivariate normal distribution

# API:
Example initialization:
```C++
Vector2d mean;
mean << 0,
        0;

Matrix2d covar;
covar << 10, 0,
          0, 1;

EigenMultivariateNormal<double> multVNorm(mean, covar);
```

To take N number of samples from the distribution (Note that the distribution has M number of dimensions):
```C++
Matrix<double, N, M> samples = multVNorm.samples(N).transpose();
```

To evaluate the probability of a given vector against the distribution:
```C++
double prob = multVNorm.eval(vector);
```

# Credit:
[https://github.com/beniz/eigenmvn](https://github.com/beniz/eigenmvn)
[https://stackoverflow.com/a/41548972/9860973](https://stackoverflow.com/a/41548972/9860973)
