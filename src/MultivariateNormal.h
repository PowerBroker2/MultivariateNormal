/*
* CREDITS:
* https://github.com/beniz/eigenmvn/
* https://stackoverflow.com/a/41548972/9860973
*/

#pragma once
#include "Arduino.h"
#include "eigen.h"
#include <Eigen/Dense>
#include <random>

using namespace Eigen;

/*
  We need a functor that can pretend it's const,
  but to be a good random number generator
  it needs mutable state.  The standard Eigen function
  Random() just calls rand(), which changes a global
  variable.
*/
namespace Eigen {
    namespace internal {
        template<typename Scalar>
        struct scalar_normal_dist_op
        {
            static std::mt19937 rng;                        // The uniform pseudo-random algorithm
            mutable std::normal_distribution<Scalar> norm; // gaussian combinator

            EIGEN_EMPTY_STRUCT_CTOR(scalar_normal_dist_op)

                template<typename Index>
            inline const Scalar operator() (Index, Index = 0) const { return norm(rng); }
            inline void seed(const uint64_t& s) { rng.seed(s); }
        };

        template<typename Scalar>
        std::mt19937 scalar_normal_dist_op<Scalar>::rng;

        template<typename Scalar>
        struct functor_traits<scalar_normal_dist_op<Scalar> >
        {
            enum { Cost = 50 * NumTraits<Scalar>::MulCost, PacketAccess = false, IsRepeatable = false };
        };

    } // end namespace internal

    /**
      Find the eigen-decomposition of the covariance matrix
      and then store it for sampling from a multi-variate normal
    */
    template<typename Scalar>
    class EigenMultivariateNormal
    {
        Matrix<Scalar, Dynamic, Dynamic> _covar;
        Matrix<Scalar, Dynamic, Dynamic> _transform;
        Matrix< Scalar, Dynamic, 1> _mean;
        internal::scalar_normal_dist_op<Scalar> randN; // Gaussian functor
        SelfAdjointEigenSolver<Matrix<Scalar, Dynamic, Dynamic> > _eigenSolver; // drawback: this creates a useless eigenSolver when using Cholesky decomposition, but it yields access to eigenvalues and vectors

    public:
        EigenMultivariateNormal(const Matrix<Scalar, Dynamic, 1>& mean, const Matrix<Scalar, Dynamic, Dynamic>& covar, const uint64_t& seed = std::mt19937::default_seed)
        {
            randN.seed(seed);
            setMean(mean);
            setCovar(covar);
        }

        void setMean(const Matrix<Scalar, Dynamic, 1>& mean) { _mean = mean; }
        void setCovar(const Matrix<Scalar, Dynamic, Dynamic>& covar)
        {
            _covar = covar;

            // Assuming that we'll be using this repeatedly,
            // compute the transformation matrix that will
            // be applied to unit-variance independent normals

            _eigenSolver = SelfAdjointEigenSolver<Matrix<Scalar, Dynamic, Dynamic> >(_covar);
            _transform = _eigenSolver.eigenvectors() * _eigenSolver.eigenvalues().cwiseMax(0).cwiseSqrt().asDiagonal();
        }

        /// Draw nn samples from the gaussian and return them
        /// as columns in a Dynamic by nn matrix
        Matrix<Scalar, Dynamic, -1> samples(int nn)
        {
            return (_transform * Matrix<Scalar, Dynamic, -1>::NullaryExpr(_covar.rows(), nn, randN)).colwise() + _mean;
        }
        
        // https://stackoverflow.com/a/41548972/9860973
        // Evaluate multivariate normal distribution at a given point
        float eval(const Eigen::VectorXd &x)
        {
            // avoid magic numbers in your code. Compilers will be able to compute this at compile time:
            const float logSqrt2Pi = 0.5*std::log(2*M_PI);
            typedef Eigen::LLT<Eigen::MatrixXd> Chol;
            Chol chol(_covar);
            const Chol::Traits::MatrixL& L = chol.matrixL();
            float quadform = (L.solve(x - _mean)).squaredNorm();
            return std::exp(-x.rows()*logSqrt2Pi - 0.5*quadform) / L.determinant();
        }
    }; // end class EigenMultivariateNormal
} // end namespace Eigen
