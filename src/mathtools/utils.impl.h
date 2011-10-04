/****************************************************************************
**
**  FougTools
**  Copyright FougSys (1 Mar. 2011)
**  contact@fougsys.fr
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language, the Qt and Open Cascade toolkits.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
**
** As a counterpart to the access to the source code and  rights to copy,
** modify and redistribute granted by the license, users are provided only
** with a limited warranty  and the software's author,  the holder of the
** economic rights,  and the successive licensors  have only  limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading,  using,  modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean  that it is complicated to manipulate,  and  that  also
** therefore means  that it is reserved for developers  and  experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards their
** requirements in conditions enabling the security of their systems and/or
** data to be ensured and,  more generally, to use and operate it in the
** same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
**
****************************************************************************/

#ifdef BITS_MATH_UTILS_H

#include <cassert>

// ---- Types Definitions


// ---- Comparison


template<typename _T_>
bool equalByAbsError(const _T_& a, const _T_& b, const _T_& tol)
{
  assert(tol >= zero<_T_>() && "positive_tolerance");
  return std::abs(a - b) <= tol;
}

template<typename _T_>
bool equalByRelError(const _T_& a, const _T_& b,
                     const _T_& maxRelError,
                     const _T_& maxAbsError)
{
  if (std::abs(a - b) < maxAbsError)
    return true;
  _T_ relativeError;
  if (std::abs(b) > std::abs(a))
    relativeError = std::abs((a - b) / b);
  else
    relativeError = std::abs((a - b) / a);
  if (relativeError <= maxRelError)
    return true;
  return false;
}

template<typename _T_>
bool equalByIntDiff(const _T_& a, const _T_& b,
             typename __impl::TypeTraits<_T_>::IntegralType_t maxDistInts,
             ComparisonCheckFlags checkFlags)
{
  typedef typename __impl::TypeTraits<_T_> Traits_t;
  typedef typename __impl::TypeTraits<_T_>::IntegralType_t TraitsInt_t;

  assert(0 < maxDistInts && "positive_and_small_enough");

  // Comparison flags handling.
  if ((checkFlags & NoCheck) == 0) {
/*    if ((checkFlags & InfinityCheck) && (isInfinite(a) || isInfinite(b)))
    {
      // a and b are infinity (positive or negative) then only return true
      // if they are exactly equal to each other that is, if they are
      // both infinities of the same sign.
      return a == b;
    }
    if ((checkFlags & NanCheck) && (isNan(a) || isNan(b)))
    {
      // If a or b is a NAN, return false. NANs are equal to nothing, not
      // even themselves.
      return false;
    }*/
    if ((checkFlags & SignCheck) && (sign(a) != sign(b))) {
      // The check for a == b is used because zero and negative zero have
      // different signs but are equal to each other.
      return a == b;
    }
  }

  // Perform the general case comparison.
  TraitsInt_t aAsInt = *(TraitsInt_t*)(&a);
  if (aAsInt < 0) {
    // Make aAsInt lexicographically ordered as a twos-complement int.
    aAsInt = Traits_t::twoComplementValue() - aAsInt;
  }
  TraitsInt_t bAsInt = *(TraitsInt_t*)(&b);
  if (bAsInt < 0) {
    // Make bAsInt lexicographically ordered as a twos-complement int.
    bAsInt = Traits_t::twoComplementValue() - bAsInt;
  }
  return std::abs(static_cast<double>(aAsInt - bAsInt)) <= maxDistInts;
}

template<typename _T_>
const _T_& minimum(const _T_& a, const _T_& b)
{
  return (b < a ? b : a);
}

template<typename _T_>
const _T_& maximum(const _T_& a, const _T_& b)
{
  return (b < a ? a : b);
}

template<typename _T_, typename _LESS_THAN_>
const _T_& minimum(const _T_& a, const _T_& b, _LESS_THAN_ lessThan)
{
  return (lessThan(b, a) ? b : a);
}

template<typename _T_, typename _LESS_THAN_>
const _T_& maximum(const _T_& a, const _T_& b, _LESS_THAN_ lessThan)
{
  return (lessThan(b, a) ? a : b);
}

template<typename _T_>
_T_ clamped(const _T_& v, const _T_& min, const _T_& max)
{
  return v < min ? min : (v > max ? max : v);
}

// ---- Conversion

template<typename _T_>
double radianToDegree(const _T_& angle)
{
  return (static_cast<double>(angle) * 180.) / math::pi;
}

template<typename _T_>
double degreeToRadian(const _T_& angle)
{
  return (math::pi * static_cast<double>(angle)) / 180.;
}

// ---- Misceallenous

template<typename _T_>
int sign(const _T_& v)
{
  if (v == zero<_T_>())
    return 0;
  typedef typename __impl::TypeTraits<_T_> Traits_t;
  typedef typename __impl::TypeTraits<_T_>::IntegralType_t TraitsInt_t;
  if (Traits_t::isNativeIntegralType())
    return v > 0 ? 1 : -1;
  else
    return ((*(TraitsInt_t*)&v) & Traits_t::twoComplementValue()) >= 0
           ? 1 : -1;
}

template<typename _T_>
_T_ square(const _T_& x)
{
  return x * x;
}

template<typename _T_>
_T_ zero()
{
  return static_cast<_T_>(0);
}

// ---- Status Report

// 
// /*! \brief Is the value \p v infinite ?
//  *  \todo  Templatize this function.
//  */
// const bool isInfinite(const float v)
// {
//   const qint32 kInfAsInt = 0x7F800000;
//   // An infinity has an exponent of 255 (shift left 23 positions) and
//   // a zero mantissa. There are two infinities - positive and negative.
//   return (*(qint32*)&v & 0x7FFFFFFF) == kInfAsInt;
// }
// 
// /*! \brief Is the value \p v a NAN (Not A Number) ?
//  *  \todo  Templatize this function.
//  */
// const bool isNan(const float v)
// {
//   // A NAN has an exponent of 255 (shifted left 23 positions) and
//   // a non-zero mantissa.
//   qint32 exp = *(qint32*) & v & 0x7F800000;
//   qint32 mantissa = *(qint32*) & v & 0x007FFFFF;
//   return exp == 0x7F800000 && mantissa != 0;
// }

#endif // BITS_MATH_UTILS_H