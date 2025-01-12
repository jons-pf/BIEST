#include <biest/surface_op.hpp>
#include <sctl.hpp>

namespace biest {

template <class Real> Surface<Real>::Surface(sctl::Long Nt, sctl::Long Np, SurfType type) : Nt0(Nt), Np0(Np) {
  auto stell_geom = [](sctl::Vector<Real>& X0, sctl::Long Nt, sctl::Long Np, Real R0, Real a, Real b, bool ellipse) {
    X0.ReInit(COORD_DIM * Nt * Np);
    sctl::Matrix<Real> X(2,1), X_(2,1);
    sctl::Matrix<Real> R(2,2), Rt(2,2);
    for (sctl::Long t = 0; t < Nt; t++) {
      R[0][0] = sctl::cos(1.5*2*sctl::const_pi<Real>()*t/Nt);
      R[0][1] = sctl::sin(1.5*2*sctl::const_pi<Real>()*t/Nt);
      R[1][0] =-sctl::sin(1.5*2*sctl::const_pi<Real>()*t/Nt);
      R[1][1] = sctl::cos(1.5*2*sctl::const_pi<Real>()*t/Nt);

      Rt[0][0] = sctl::cos(1.5*2*sctl::const_pi<Real>()*t/Nt);
      Rt[1][0] = sctl::sin(1.5*2*sctl::const_pi<Real>()*t/Nt);
      Rt[0][1] =-sctl::sin(1.5*2*sctl::const_pi<Real>()*t/Nt);
      Rt[1][1] = sctl::cos(1.5*2*sctl::const_pi<Real>()*t/Nt);
      for (sctl::Long p = 0; p < Np; p++) {
        X[0][0] = sctl::cos(2 * sctl::const_pi<Real>() * p / Np);
        X[1][0] = sctl::sin(2 * sctl::const_pi<Real>() * p / Np);
        if (!ellipse) X[0][0] = (X[0][0] + sctl::exp(sctl::cos(2 * sctl::const_pi<Real>() * p / Np)) - 1.5) / 2.4;

        sctl::Matrix<Real>::GEMM(X_, R, X);
        X_[0][0] *= a;
        X_[1][0] *= b;
        sctl::Matrix<Real>::GEMM(X, Rt, X_);

        X0[(0 * Nt + t) * Np + p] = (X[0][0] + (Real)R0) * sctl::cos(2 * sctl::const_pi<Real>() * t / Nt);
        X0[(1 * Nt + t) * Np + p] = (X[0][0] + (Real)R0) * sctl::sin(2 * sctl::const_pi<Real>() * t / Nt);
        X0[(2 * Nt + t) * Np + p] = (X[1][0] + (Real) 0);
      }
    }
  };
  if (type == SurfType::AxisymCircleWide) {
    stell_geom(X0_, Nt0, Np0, 2.0, 1.0, 1.0, true);
  } else if (type == SurfType::AxisymCircleNarrow) {
    stell_geom(X0_, Nt0, Np0, 2.0, 0.5, 0.5, true);
  } else if (type == SurfType::AxisymWide) {
    stell_geom(X0_, Nt0, Np0, 2.0, 1.0, 1.0, false);
  } else if (type == SurfType::AxisymNarrow) {
    stell_geom(X0_, Nt0, Np0, 2.0, 0.5, 0.5, false);
  } else if (type == SurfType::RotatingEllipseWide) {
    stell_geom(X0_, Nt0, Np0, 2.0, 0.7, 1.0, true);
  } else if (type == SurfType::RotatingEllipseNarrow) {
    stell_geom(X0_, Nt0, Np0, 2.0, 0.3, 0.55, true);
  } else if (type == SurfType::Quas3) {
    sctl::Matrix<double> X, Y, Z;
    X.Read("geom/Quas3-X.mat");
    Y.Read("geom/Quas3-Y.mat");
    Z.Read("geom/Quas3-Z.mat");
    sctl::Long Nt = X.Dim(0)-1, Np = X.Dim(1)-1;
    sctl::Vector<Real> XX(COORD_DIM * Nt * Np);
    for (sctl::Long t = 0; t < Nt; t++) {
      for (sctl::Long p = 0; p < Np; p++) {
        XX[(0 * Nt + t) * Np + p] = (Real)X[t][p] * 0.45;
        XX[(1 * Nt + t) * Np + p] = (Real)Y[t][p] * 0.45;
        XX[(2 * Nt + t) * Np + p] = (Real)Z[t][p] * 0.45;
      }
    }
    SurfaceOp<Real>::Upsample(XX, Nt, Np, this->Coord(), Nt0, Np0);
  } else if (type == SurfType::LHD) {
    sctl::Matrix<double> X, Y, Z;
    X.Read("geom/LHD-X.mat");
    Y.Read("geom/LHD-Y.mat");
    Z.Read("geom/LHD-Z.mat");
    sctl::Long Nt = X.Dim(0), Np = X.Dim(1);
    sctl::Vector<Real> XX(COORD_DIM * Nt * Np);
    for (sctl::Long t = 0; t < Nt; t++) {
      for (sctl::Long p = 0; p < Np; p++) {
        XX[(0 * Nt + t) * Np + p] = (Real)X[t][p] * 0.25;
        XX[(1 * Nt + t) * Np + p] = (Real)Y[t][p] * 0.25;
        XX[(2 * Nt + t) * Np + p] = (Real)Z[t][p] * 0.25;
      }
    }
    SurfaceOp<Real>::Upsample(XX, Nt, Np, this->Coord(), Nt0, Np0);
  } else if (type == SurfType::W7X) {
    sctl::Matrix<double> X, Y, Z;
    X.Read("geom/W7X-X.mat");
    Y.Read("geom/W7X-Y.mat");
    Z.Read("geom/W7X-Z.mat");
    sctl::Long Nt = X.Dim(0), Np = X.Dim(1);
    sctl::Vector<Real> XX(COORD_DIM * Nt * Np);
    for (sctl::Long t = 0; t < Nt; t++) {
      for (sctl::Long p = 0; p < Np; p++) {
        XX[(0 * Nt + t) * Np + p] = (Real)X[t][p] * 0.45;
        XX[(1 * Nt + t) * Np + p] = (Real)Y[t][p] * 0.45;
        XX[(2 * Nt + t) * Np + p] = (Real)Z[t][p] * 0.45;
      }
    }
    SurfaceOp<Real>::Upsample(XX, Nt, Np, this->Coord(), Nt0, Np0);
  } else if (type == SurfType::Stell) {
    sctl::Matrix<Real> Rcoeff(2,2), Zcoeff(2,2);
    Rcoeff = 0;
    Zcoeff = 0;
    Rcoeff[0][0] =  1.0e+01;
    Rcoeff[0][1] =  1.0e+00;
    Rcoeff[1][1] =  2.5e-01;
    Zcoeff[0][1] = -1.0e+00;
    Zcoeff[1][1] =  2.5e-01;

    X0_.ReInit(COORD_DIM * Nt0 * Np0);
    for (long t = 0; t < Nt0; t++) {
      for (long p = 0; p < Np0; p++) {
        Real theta = t * 2 * sctl::const_pi<Real>() / Nt0;
        Real phi   = p * 2 * sctl::const_pi<Real>() / Np0;
        Real R = 0, Z = 0;
        for (long i = 0; i < 2; i++) {
          for (long j = 0; j < 2; j++) {
            R += Rcoeff[i][j] * sctl::cos(j*phi - 5*i*theta);
            Z += Zcoeff[i][j] * sctl::sin(j*phi - 5*i*theta);
          }
        }
        X0_[0*Nt0*Np0 + t*Np0 + p] = R * sctl::cos(theta);
        X0_[1*Nt0*Np0 + t*Np0 + p] = R * sctl::sin(theta);
        X0_[2*Nt0*Np0 + t*Np0 + p] = Z;
      }
    }
  } else if (type == SurfType::W7X_) {
    sctl::Matrix<Real> Rcoeff(21,21), Zcoeff(21,21);
    Rcoeff = 0;
    Zcoeff = 0;
    auto Rbc = [&Rcoeff](sctl::Long i, sctl::Long j) ->Real& { return Rcoeff[i+10][j+10]; };
    auto Zbs = [&Zcoeff](sctl::Long i, sctl::Long j) ->Real& { return Zcoeff[i+10][j+10]; };
    { // Set Rcoeff, Zcoeff
      Rbc(0,0)    =  5.499222200000000E+00; Zbs(0,0)    =  0.000000000000000E+00;
      Rbc(1,0)    =  2.862415760000000E-01; Zbs(1,0)    = -2.476328720000000E-01;
      Rbc(2,0)    = -2.821010320000000E-03; Zbs(2,0)    = -3.798656180000000E-03;
      Rbc(3,0)    =  4.622923140000000E-04; Zbs(3,0)    =  1.379383388000000E-03;
      Rbc(4,0)    = -6.360328500000000E-04; Zbs(4,0)    =  6.992897620000000E-04;
      Rbc(5,0)    = -1.135627524000000E-04; Zbs(5,0)    = -3.409457720000000E-05;
      Rbc(6,0)    = -1.816774936000000E-04; Zbs(6,0)    = -1.255490184000000E-04;
      Rbc(7,0)    =  1.341661870000000E-04; Zbs(7,0)    =  1.037604320000000E-04;
      Rbc(8,0)    = -2.663143840000000E-04; Zbs(8,0)    = -4.829788340000000E-04;
      Rbc(9,0)    =  7.395449780000000E-05; Zbs(9,0)    =  2.269942520000000E-04;
      Rbc(10,0)   =  1.213701750000000E-04; Zbs(10,0)   =  1.063213162000000E-04;
      Rbc(-10,1)  =  3.149812680000000E-05; Zbs(-10,1)  =  5.847707230000000E-05;
      Rbc(-8,1)   = -3.424627190000000E-04; Zbs(-8,1)   =  2.389303750000000E-04;
      Rbc(-7,1)   = -5.528744530000000E-05; Zbs(-7,1)   = -1.910728510000000E-04;
      Rbc(-6,1)   =  6.117681689999999E-05; Zbs(-6,1)   = -5.092602740000000E-04;
      Rbc(-5,1)   =  3.696634690000000E-04; Zbs(-5,1)   = -3.176068510000000E-04;
      Rbc(-4,1)   =  2.191307230000000E-04; Zbs(-4,1)   =  7.293541070000000E-04;
      Rbc(-3,1)   =  1.269562650000000E-03; Zbs(-3,1)   = -6.193015160000001E-04;
      Rbc(-2,1)   = -2.148173690000000E-02; Zbs(-2,1)   = -2.227519550000000E-02;
      Rbc(-1,1)   = -2.080619220000000E-01; Zbs(-1,1)   = -1.793381930000000E-01;
      Rbc(0,1)    =  4.591451160000000E-01; Zbs(0,1)    = -6.108126480000000E-01;
      Rbc(1,1)    =  3.971068830000000E-02; Zbs(1,1)    = -3.707128920000000E-02;
      Rbc(2,1)    =  3.618299070000000E-03; Zbs(2,1)    = -5.337647490000000E-03;
      Rbc(3,1)    = -3.891312730000000E-04; Zbs(3,1)    = -3.850478840000000E-04;
      Rbc(4,1)    =  4.101864840000000E-04; Zbs(4,1)    = -5.420620840000000E-04;
      Rbc(5,1)    =  3.192006150000000E-05; Zbs(5,1)    =  5.585418590000000E-04;
      Rbc(6,1)    = -5.386882030000000E-04; Zbs(6,1)    = -2.760504880000000E-04;
      Rbc(7,1)    = -3.173680500000000E-04; Zbs(7,1)    =  7.432511449999999E-05;
      Rbc(8,1)    =  4.024401190000000E-04; Zbs(8,1)    = -1.269831920000000E-04;
      Rbc(9,1)    =  2.130320640000000E-04; Zbs(9,1)    = -1.203549150000000E-04;
      Rbc(10,1)   =  1.636947630000000E-04; Zbs(10,1)   =  8.987937330000000E-05;
      Rbc(-10,2)  = -7.765489310000000E-05; Zbs(-10,2)  = -8.678722490000000E-05;
      Rbc(-8,2)   =  4.456479760000000E-05; Zbs(-8,2)   =  1.145912760000000E-04;
      Rbc(-7,2)   =  5.600939840000000E-05; Zbs(-7,2)   = -2.025932200000000E-06;
      Rbc(-6,2)   =  3.165439980000000E-05; Zbs(-6,2)   =  3.892001780000000E-05;
      Rbc(-5,2)   =  2.876781000000000E-04; Zbs(-5,2)   =  3.010497420000000E-04;
      Rbc(-4,2)   = -3.334248370000000E-04; Zbs(-4,2)   =  2.345406280000000E-04;
      Rbc(-3,2)   =  5.907835000000000E-04; Zbs(-3,2)   =  2.120520070000000E-04;
      Rbc(-2,2)   =  5.679743510000000E-02; Zbs(-2,2)   =  5.802810780000000E-02;
      Rbc(-1,2)   =  3.730642040000000E-02; Zbs(-1,2)   = -2.576171930000000E-02;
      Rbc(0,2)    =  5.279093270000000E-02; Zbs(0,2)    = -1.753837990000000E-02;
      Rbc(1,2)    =  1.057394420000000E-02; Zbs(1,2)    = -5.454225200000000E-03;
      Rbc(2,2)    =  2.688744090000000E-03; Zbs(2,2)    = -2.870790520000000E-04;
      Rbc(3,2)    =  8.698832710000000E-04; Zbs(3,2)    = -6.835677030000000E-04;
      Rbc(4,2)    =  1.170861370000000E-04; Zbs(4,2)    = -2.436806940000000E-04;
      Rbc(5,2)    = -3.468535580000000E-04; Zbs(5,2)    = -3.554177930000000E-04;
      Rbc(6,2)    =  3.854471210000000E-04; Zbs(6,2)    =  4.853309450000000E-04;
      Rbc(7,2)    = -1.035513210000000E-04; Zbs(7,2)    = -3.634096180000000E-04;
      Rbc(8,2)    = -1.028097890000000E-04; Zbs(8,2)    =  1.530850250000000E-04;
      Rbc(9,2)    =  1.632011900000000E-04; Zbs(9,2)    = -1.707748690000000E-04;
      Rbc(10,2)   =  1.581787490000000E-04; Zbs(10,2)   = -3.585159010000000E-05;
      Rbc(-10,3)  =  4.771322000000000E-05; Zbs(-10,3)  =  9.188864070000000E-05;
      Rbc(-8,3)   = -6.313692220000000E-05; Zbs(-8,3)   = -2.572672720000000E-05;
      Rbc(-7,3)   =  5.097030830000000E-05; Zbs(-7,3)   =  3.932101880000000E-05;
      Rbc(-6,3)   = -9.952454240000000E-05; Zbs(-6,3)   = -9.632583980000000E-05;
      Rbc(-5,3)   = -2.012500210000000E-04; Zbs(-5,3)   = -1.439074920000000E-04;
      Rbc(-4,3)   = -5.638133750000001E-04; Zbs(-4,3)   = -3.602899270000000E-04;
      Rbc(-3,3)   = -1.249071190000000E-02; Zbs(-3,3)   = -1.229819730000000E-02;
      Rbc(-2,3)   = -1.470456180000000E-02; Zbs(-2,3)   = -1.098330420000000E-02;
      Rbc(-1,3)   = -2.963558930000000E-03; Zbs(-1,3)   = -3.655102520000000E-03;
      Rbc(0,3)    =  3.531728600000000E-03; Zbs(0,3)    = -1.279854780000000E-03;
      Rbc(1,3)    =  1.401374340000000E-03; Zbs(1,3)    =  1.590869290000000E-03;
      Rbc(2,3)    = -1.273655750000000E-04; Zbs(2,3)    = -1.929131170000000E-04;
      Rbc(3,3)    =  7.079597950000001E-05; Zbs(3,3)    = -1.224304200000000E-04;
      Rbc(4,3)    =  2.850121260000000E-04; Zbs(4,3)    =  2.598242880000000E-04;
      Rbc(5,3)    =  1.999424460000000E-04; Zbs(5,3)    =  1.123146280000000E-04;
      Rbc(6,3)    = -3.092845690000000E-04; Zbs(6,3)    = -2.016395580000000E-04;
      Rbc(7,3)    = -5.220858140000000E-05; Zbs(7,3)    =  1.316453710000000E-04;
      Rbc(8,3)    =  2.440456610000000E-04; Zbs(8,3)    =  3.632665850000000E-05;
      Rbc(9,3)    =  1.636232450000000E-04; Zbs(9,3)    = -6.465022640000000E-05;
      Rbc(10,3)   =  6.580775879999999E-05; Zbs(10,3)   = -5.470819060000000E-05;
      Rbc(-10,4)  = -1.584638410000000E-06; Zbs(-10,4)  =  6.451981970000000E-06;
      Rbc(-8,4)   = -1.656855160000000E-05; Zbs(-8,4)   = -7.496657650000000E-05;
      Rbc(-7,4)   =  1.657078710000000E-05; Zbs(-7,4)   =  2.015102120000000E-05;
      Rbc(-6,4)   =  1.152800160000000E-04; Zbs(-6,4)   =  5.273493610000000E-05;
      Rbc(-5,4)   =  1.587317930000000E-04; Zbs(-5,4)   = -1.770118060000000E-06;
      Rbc(-4,4)   =  1.693684220000000E-03; Zbs(-4,4)   =  1.459908040000000E-03;
      Rbc(-3,4)   =  5.456882450000000E-03; Zbs(-3,4)   =  5.427735220000000E-03;
      Rbc(-2,4)   =  1.225123000000000E-03; Zbs(-2,4)   = -5.642061490000000E-04;
      Rbc(-1,4)   = -1.634672880000000E-03; Zbs(-1,4)   = -1.759500600000000E-03;
      Rbc(0,4)    =  1.972846670000000E-03; Zbs(0,4)    = -9.862657130000000E-04;
      Rbc(1,4)    = -3.061925850000000E-04; Zbs(1,4)    =  6.134663420000001E-04;
      Rbc(2,4)    = -2.248673790000000E-04; Zbs(2,4)    =  9.844427600000000E-05;
      Rbc(3,4)    =  1.440742170000000E-04; Zbs(3,4)    =  2.386755350000000E-04;
      Rbc(4,4)    = -3.970536280000000E-05; Zbs(4,4)    = -5.037994770000000E-05;
      Rbc(5,4)    = -1.067147990000000E-04; Zbs(5,4)    =  5.683708790000000E-05;
      Rbc(6,4)    =  8.551267700000000E-05; Zbs(6,4)    =  5.857943570000000E-05;
      Rbc(7,4)    = -1.050568620000000E-04; Zbs(7,4)    = -1.372536220000000E-04;
      Rbc(8,4)    = -5.167821720000000E-05; Zbs(8,4)    = -7.182784940000000E-05;
      Rbc(9,4)    =  2.259193970000000E-06; Zbs(9,4)    = -2.070346460000000E-05;
      Rbc(10,4)   =  2.383410400000000E-06; Zbs(10,4)   = -1.711598370000000E-05;
      Rbc(-10,5)  = -1.054277140000000E-05; Zbs(-10,5)  = -4.590453350000000E-05;
      Rbc(-8,5)   =  1.840472560000000E-05; Zbs(-8,5)   =  2.408429910000000E-05;
      Rbc(-7,5)   = -2.421259200000000E-05; Zbs(-7,5)   = -3.838815110000000E-05;
      Rbc(-6,5)   = -7.576990619999999E-05; Zbs(-6,5)   = -3.713491020000000E-05;
      Rbc(-5,5)   = -2.754279550000000E-04; Zbs(-5,5)   = -2.168078010000000E-04;
      Rbc(-4,5)   = -1.706462950000000E-03; Zbs(-4,5)   = -1.644204180000000E-03;
      Rbc(-3,5)   = -1.313493520000000E-03; Zbs(-3,5)   = -9.164101610000000E-04;
      Rbc(-2,5)   =  1.549990410000000E-04; Zbs(-2,5)   =  3.455799460000000E-04;
      Rbc(-1,5)   = -1.044131360000000E-03; Zbs(-1,5)   = -4.688486220000000E-05;
      Rbc(0,5)    =  6.811915640000000E-04; Zbs(0,5)    = -5.596368950000000E-04;
      Rbc(1,5)    = -2.239129020000000E-04; Zbs(1,5)    =  1.351560670000000E-04;
      Rbc(2,5)    = -9.469807639999999E-05; Zbs(2,5)    =  1.702559280000000E-05;
      Rbc(3,5)    = -2.114701210000000E-05; Zbs(3,5)    = -3.835918030000000E-06;
      Rbc(4,5)    =  1.354520640000000E-04; Zbs(4,5)    =  1.413910340000000E-04;
      Rbc(5,5)    = -1.082798510000000E-04; Zbs(5,5)    = -1.868524930000000E-04;
      Rbc(6,5)    = -4.385285270000000E-05; Zbs(6,5)    =  3.311395990000000E-06;
      Rbc(7,5)    = -1.993355920000000E-05; Zbs(7,5)    = -2.425407170000000E-05;
      Rbc(8,5)    = -1.381184910000000E-05; Zbs(8,5)    = -3.354113410000000E-05;
      Rbc(9,5)    = -8.047951870000001E-06; Zbs(9,5)    = -2.617354950000000E-05;
      Rbc(10,5)   =  5.442943530000000E-06; Zbs(10,5)   = -7.540077230000000E-06;
      Rbc(-10,6)  =  5.897904990000000E-06; Zbs(-10,6)  =  1.015941240000000E-05;
      Rbc(-8,6)   = -1.711315030000000E-06; Zbs(-8,6)   =  1.460063060000000E-05;
      Rbc(-7,6)   =  2.779279110000000E-05; Zbs(-7,6)   =  9.346117740000001E-06;
      Rbc(-6,6)   =  6.159498210000000E-05; Zbs(-6,6)   =  3.757257530000000E-05;
      Rbc(-5,6)   =  3.838807420000000E-04; Zbs(-5,6)   =  3.538485060000000E-04;
      Rbc(-4,6)   =  5.812892710000000E-04; Zbs(-4,6)   =  5.854490200000000E-04;
      Rbc(-3,6)   = -1.282036420000000E-05; Zbs(-3,6)   = -9.995316180000000E-05;
      Rbc(-2,6)   =  3.747466590000000E-04; Zbs(-2,6)   =  4.378838950000000E-04;
      Rbc(-1,6)   =  4.290153220000000E-04; Zbs(-1,6)   = -7.532518330000000E-04;
      Rbc(0,6)    =  8.328937110000000E-04; Zbs(0,6)    = -6.075503770000000E-04;
      Rbc(1,6)    = -1.772339600000000E-05; Zbs(1,6)    = -7.259493060000000E-05;
      Rbc(2,6)    =  3.629202880000000E-05; Zbs(2,6)    =  5.097636310000000E-05;
      Rbc(3,6)    =  9.744514720000001E-06; Zbs(3,6)    = -2.890839380000000E-05;
      Rbc(4,6)    =  3.762214890000000E-06; Zbs(4,6)    = -2.016824820000000E-05;
      Rbc(5,6)    = -2.473214420000000E-05; Zbs(5,6)    =  4.460497530000000E-06;
      Rbc(6,6)    =  9.393233570000001E-06; Zbs(6,6)    = -1.180516910000000E-05;
      Rbc(7,6)    = -7.516850280000000E-06; Zbs(7,6)    = -1.452989350000000E-05;
      Rbc(8,6)    = -1.256492920000000E-06; Zbs(8,6)    = -1.179898990000000E-05;
      Rbc(9,6)    = -3.141617150000000E-06; Zbs(9,6)    = -1.756033910000000E-05;
      Rbc(10,6)   = -2.246448250000000E-06; Zbs(10,6)   = -9.158244980000000E-06;
      Rbc(-10,7)  =  5.929060270000000E-06; Zbs(-10,7)  = -1.076521400000000E-05;
      Rbc(-8,7)   = -4.097376180000000E-06; Zbs(-8,7)   = -7.849626889999999E-06;
      Rbc(-7,7)   = -1.789842870000000E-05; Zbs(-7,7)   = -5.481835500000000E-06;
      Rbc(-6,7)   = -7.895979130000000E-05; Zbs(-6,7)   = -6.009604550000000E-05;
      Rbc(-5,7)   = -1.699260490000000E-04; Zbs(-5,7)   = -2.072394130000000E-04;
      Rbc(-4,7)   = -6.638575870000000E-05; Zbs(-4,7)   = -3.656338330000000E-05;
      Rbc(-3,7)   =  9.249536449999999E-05; Zbs(-3,7)   =  1.022424950000000E-04;
      Rbc(-2,7)   =  1.172203510000000E-03; Zbs(-2,7)   =  1.077845730000000E-03;
      Rbc(-1,7)   = -2.573141910000000E-03; Zbs(-1,7)   =  3.037810800000000E-03;
      Rbc(0,7)    =  6.449810380000000E-05; Zbs(0,7)    = -3.584656230000000E-05;
      Rbc(1,7)    =  2.190318120000000E-05; Zbs(1,7)    = -2.286948060000000E-05;
      Rbc(2,7)    =  1.722638980000000E-05; Zbs(2,7)    = -1.325952420000000E-05;
      Rbc(3,7)    = -2.541553570000000E-05; Zbs(3,7)    =  4.086201040000000E-06;
      Rbc(4,7)    =  1.336724870000000E-05; Zbs(4,7)    = -2.288792230000000E-05;
      Rbc(5,7)    = -7.251710020000000E-06; Zbs(5,7)    = -1.480782940000000E-05;
      Rbc(6,7)    =  7.086029940000000E-06; Zbs(6,7)    =  3.959755080000000E-07;
      Rbc(7,7)    = -2.160257930000000E-05; Zbs(7,7)    =  1.387646540000000E-05;
      Rbc(8,7)    = -7.244894970000000E-06; Zbs(8,7)    =  5.399568120000000E-06;
      Rbc(9,7)    = -3.206902820000000E-06; Zbs(9,7)    = -6.539029700000000E-06;
      Rbc(10,7)   =  1.060093110000000E-05; Zbs(10,7)   = -9.468079560000000E-06;
      Rbc(-10,8)  = -1.145540760000000E-06; Zbs(-10,8)  = -1.426725280000000E-05;
      Rbc(-8,8)   =  8.075915779999999E-07; Zbs(-8,8)   = -2.090075330000000E-06;
      Rbc(-7,8)   =  1.225482070000000E-05; Zbs(-7,8)   =  1.002867410000000E-05;
      Rbc(-6,8)   =  4.185049880000000E-05; Zbs(-6,8)   =  3.627089890000000E-05;
      Rbc(-5,8)   =  5.202132510000000E-05; Zbs(-5,8)   =  3.298637690000000E-05;
      Rbc(-4,8)   = -1.790487510000000E-05; Zbs(-4,8)   = -2.696253960000000E-05;
      Rbc(-3,8)   = -6.164140990000000E-04; Zbs(-3,8)   = -6.284073990000000E-04;
      Rbc(-2,8)   = -3.576911990000000E-04; Zbs(-2,8)   =  3.465212410000000E-04;
      Rbc(-1,8)   = -5.728932250000000E-04; Zbs(-1,8)   =  3.377205970000000E-04;
      Rbc(0,8)    = -8.260321650000000E-05; Zbs(0,8)    =  5.832294860000000E-05;
      Rbc(1,8)    = -3.522536740000000E-05; Zbs(1,8)    =  2.406038780000000E-05;
      Rbc(2,8)    =  2.684092340000000E-07; Zbs(2,8)    =  1.991160860000000E-05;
      Rbc(3,8)    =  1.192629450000000E-06; Zbs(3,8)    =  1.025583730000000E-07;
      Rbc(4,8)    =  1.060654930000000E-05; Zbs(4,8)    =  8.367961500000000E-06;
      Rbc(5,8)    = -1.069578210000000E-05; Zbs(5,8)    = -1.167514120000000E-05;
      Rbc(6,8)    =  5.935593950000000E-07; Zbs(6,8)    =  6.173553520000000E-06;
      Rbc(7,8)    = -1.782560770000000E-06; Zbs(7,8)    =  4.560812670000000E-07;
      Rbc(8,8)    = -2.019758340000000E-06; Zbs(8,8)    =  5.240396660000000E-06;
      Rbc(9,8)    = -2.664745030000000E-06; Zbs(9,8)    = -2.290649840000000E-06;
      Rbc(10,8)   =  2.187762770000000E-06; Zbs(10,8)   =  2.349449310000000E-06;
      Rbc(-10,9)  =  1.072796270000000E-06; Zbs(-10,9)  =  7.710368280000000E-07;
      Rbc(-8,9)   = -3.954187380000000E-06; Zbs(-8,9)   = -1.287612760000000E-06;
      Rbc(-7,9)   = -5.975561270000000E-06; Zbs(-7,9)   = -3.802088860000000E-06;
      Rbc(-6,9)   = -9.711522580000000E-06; Zbs(-6,9)   = -1.111428870000000E-05;
      Rbc(-5,9)   =  1.283299810000000E-05; Zbs(-5,9)   =  1.270473750000000E-05;
      Rbc(-4,9)   =  2.050599600000000E-04; Zbs(-4,9)   =  2.090269370000000E-04;
      Rbc(-3,9)   =  2.150777160000000E-04; Zbs(-3,9)   =  1.663527070000000E-04;
      Rbc(-2,9)   =  4.308369360000000E-05; Zbs(-2,9)   =  1.255918080000000E-04;
      Rbc(-1,9)   = -4.735076660000000E-05; Zbs(-1,9)   =  2.779910190000000E-06;
      Rbc(0,9)    =  1.787987800000000E-05; Zbs(0,9)    = -1.003487210000000E-04;
      Rbc(1,9)    = -3.850292090000000E-06; Zbs(1,9)    = -1.737272460000000E-06;
      Rbc(2,9)    = -6.038516310000000E-06; Zbs(2,9)    = -1.027369980000000E-05;
      Rbc(3,9)    = -5.018549580000000E-06; Zbs(3,9)    = -1.159928700000000E-05;
      Rbc(4,9)    =  4.292816850000000E-06; Zbs(4,9)    = -3.788971130000000E-06;
      Rbc(5,9)    =  6.944764760000000E-06; Zbs(5,9)    = -5.514237360000000E-07;
      Rbc(6,9)    =  4.340471530000000E-06; Zbs(6,9)    = -5.532296800000000E-06;
      Rbc(7,9)    = -5.070625820000000E-06; Zbs(7,9)    = -4.607223520000000E-06;
      Rbc(8,9)    = -2.124002090000000E-06; Zbs(8,9)    = -3.273312010000000E-06;
      Rbc(9,9)    = -2.177449410000000E-07; Zbs(9,9)    = -5.030283200000000E-06;
      Rbc(10,9)   = -4.607967820000000E-07; Zbs(10,9)   = -3.725635690000000E-06;
      Rbc(-10,10) = -1.281240510000000E-06; Zbs(-10,10) = -6.404339920000000E-06;
      Rbc(-8,10)  =  1.956168450000000E-06; Zbs(-8,10)  = -1.805927600000000E-06;
      Rbc(-7,10)  =  1.101080660000000E-06; Zbs(-7,10)  = -3.471541870000000E-07;
      Rbc(-6,10)  = -3.993429860000000E-06; Zbs(-6,10)  = -4.649932760000000E-06;
      Rbc(-5,10)  = -3.274758810000000E-05; Zbs(-5,10)  = -3.510246990000000E-05;
      Rbc(-4,10)  = -1.059813770000000E-04; Zbs(-4,10)  = -1.105165310000000E-04;
      Rbc(-3,10)  =  1.095947770000000E-05; Zbs(-3,10)  =  4.305883900000000E-05;
      Rbc(-2,10)  =  2.631080330000000E-04; Zbs(-2,10)  =  2.624212580000000E-04;
      Rbc(-1,10)  = -1.383145710000000E-04; Zbs(-1,10)  =  1.209196340000000E-04;
      Rbc(0,10)   =  1.047045790000000E-05; Zbs(0,10)   = -2.287772640000000E-05;
      Rbc(1,10)   =  2.905086960000000E-06; Zbs(1,10)   =  1.019706800000000E-06;
      Rbc(2,10)   = -7.151778040000000E-07; Zbs(2,10)   = -3.808867920000000E-06;
      Rbc(3,10)   = -2.321896650000000E-06; Zbs(3,10)   = -7.137445440000000E-07;
      Rbc(4,10)   =  1.611615800000000E-06; Zbs(4,10)   = -6.657593750000000E-06;
      Rbc(5,10)   = -3.690408440000000E-06; Zbs(5,10)   = -4.748117770000000E-06;
      Rbc(6,10)   =  4.171438220000000E-06; Zbs(6,10)   =  7.838792440000000E-07;
      Rbc(7,10)   =  1.512182800000000E-06; Zbs(7,10)   =  1.488982220000000E-06;
      Rbc(8,10)   = -9.929365099999999E-07; Zbs(8,10)   = -3.261759890000000E-06;
      Rbc(9,10)   = -1.627494720000000E-06; Zbs(9,10)   = -4.826840650000000E-06;
      Rbc(10,10)  =  2.288831480000000E-06; Zbs(10,10)  = -2.453483540000000E-06;
    }
    X0_.ReInit(COORD_DIM * Nt0 * Np0);
    for (long t = 0; t < Nt0; t++) {
      for (long p = 0; p < Np0; p++) {
        Real theta = t * 2 * sctl::const_pi<Real>() / Nt0;
        Real phi   = p * 2 * sctl::const_pi<Real>() / Np0;
        Real R = 0, Z = 0;
        for (long i = -10; i <= 10; i++) {
          for (long j = -10; j <= 10; j++) {
            R += Rbc(i,j) * sctl::cos(j*phi - 5*i*theta);
            Z += Zbs(i,j) * sctl::sin(j*phi - 5*i*theta);
          }
        }
        X0_[0*Nt0*Np0 + t*Np0 + p] = R * sctl::cos(theta);
        X0_[1*Nt0*Np0 + t*Np0 + p] = R * sctl::sin(theta);
        X0_[2*Nt0*Np0 + t*Np0 + p] = Z;
      }
    }
  } else if (type == SurfType::None) {
    X0_.ReInit(COORD_DIM * Nt0 * Np0);
  } else {
    SCTL_ASSERT_MSG(false, "Unknown surface type");
  }
}

struct VTKData {
  typedef double VTKReal;
  static constexpr sctl::Integer COORD_DIM = 3;

  std::vector<VTKReal> point_coord;
  std::vector<VTKReal> point_value;
  std::vector<int32_t> line_connect;
  std::vector<int32_t> line_offset;
  std::vector<int32_t> poly_connect;
  std::vector<int32_t> poly_offset;

  void WriteVTK(const char* fname, const sctl::Comm& comm = sctl::Comm::Self()) {
    sctl::Integer np = comm.Size();
    sctl::Integer myrank = comm.Rank();

    std::vector<VTKReal>& coord=this->point_coord;
    std::vector<VTKReal>& value=this->point_value;
    std::vector<int32_t>& line_connect=this->line_connect;
    std::vector<int32_t>& line_offset=this->line_offset;
    std::vector<int32_t>& poly_connect=this->poly_connect;
    std::vector<int32_t>& poly_offset=this->poly_offset;

    sctl::Long pt_cnt=coord.size()/COORD_DIM;
    sctl::Long line_cnt=line_offset.size();
    sctl::Long poly_cnt=poly_offset.size();

    // Open file for writing.
    std::stringstream vtufname;
    vtufname<<fname<<"_"<<std::setfill('0')<<std::setw(6)<<myrank<<".vtp";
    std::ofstream vtufile;
    vtufile.open(vtufname.str().c_str());
    if(vtufile.fail()) return;

    bool isLittleEndian;
    { // Set isLittleEndian
      uint16_t number = 0x1;
      uint8_t *numPtr = (uint8_t*)&number;
      isLittleEndian=(numPtr[0] == 1);
    }

    // Proceed to write to file.
    sctl::Long data_size=0;
    vtufile<<"<?xml version=\"1.0\"?>\n";
    if(isLittleEndian) vtufile<<"<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    else               vtufile<<"<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"BigEndian\">\n";
    //===========================================================================
    vtufile<<"  <PolyData>\n";
    vtufile<<"    <Piece NumberOfPoints=\""<<pt_cnt<<"\" NumberOfVerts=\"0\" NumberOfLines=\""<<line_cnt<<"\" NumberOfStrips=\"0\" NumberOfPolys=\""<<poly_cnt<<"\">\n";

    //---------------------------------------------------------------------------
    vtufile<<"      <Points>\n";
    vtufile<<"        <DataArray type=\"Float"<<sizeof(VTKReal)*8<<"\" NumberOfComponents=\""<<COORD_DIM<<"\" Name=\"Position\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
    data_size+=sizeof(uint32_t)+coord.size()*sizeof(VTKReal);
    vtufile<<"      </Points>\n";
    //---------------------------------------------------------------------------
    if(value.size()){ // value
      vtufile<<"      <PointData>\n";
      vtufile<<"        <DataArray type=\"Float"<<sizeof(VTKReal)*8<<"\" NumberOfComponents=\""<<value.size()/pt_cnt<<"\" Name=\""<<"value"<<"\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
      data_size+=sizeof(uint32_t)+value.size()*sizeof(VTKReal);
      vtufile<<"      </PointData>\n";
    }
    //---------------------------------------------------------------------------
    vtufile<<"      <Lines>\n";
    vtufile<<"        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
    data_size+=sizeof(uint32_t)+line_connect.size()*sizeof(int32_t);
    vtufile<<"        <DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
    data_size+=sizeof(uint32_t)+line_offset.size() *sizeof(int32_t);
    vtufile<<"      </Lines>\n";
    //---------------------------------------------------------------------------
    vtufile<<"      <Polys>\n";
    vtufile<<"        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
    data_size+=sizeof(uint32_t)+poly_connect.size()*sizeof(int32_t);
    vtufile<<"        <DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\""<<data_size<<"\" />\n";
    data_size+=sizeof(uint32_t)+poly_offset.size() *sizeof(int32_t);
    vtufile<<"      </Polys>\n";
    //---------------------------------------------------------------------------

    vtufile<<"    </Piece>\n";
    vtufile<<"  </PolyData>\n";
    //===========================================================================
    vtufile<<"  <AppendedData encoding=\"raw\">\n";
    vtufile<<"    _";

    int32_t block_size;
    block_size=coord.size()*sizeof(VTKReal); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&coord  [0], coord.size()*sizeof(VTKReal));
    if(value.size()){ // value
      block_size=value.size()*sizeof(VTKReal); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&value  [0], value.size()*sizeof(VTKReal));
    }
    block_size=line_connect.size()*sizeof(int32_t); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&line_connect[0], line_connect.size()*sizeof(int32_t));
    block_size=line_offset .size()*sizeof(int32_t); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&line_offset [0], line_offset .size()*sizeof(int32_t));
    block_size=poly_connect.size()*sizeof(int32_t); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&poly_connect[0], poly_connect.size()*sizeof(int32_t));
    block_size=poly_offset .size()*sizeof(int32_t); vtufile.write((char*)&block_size, sizeof(int32_t)); vtufile.write((char*)&poly_offset [0], poly_offset .size()*sizeof(int32_t));

    vtufile<<"\n";
    vtufile<<"  </AppendedData>\n";
    //===========================================================================
    vtufile<<"</VTKFile>\n";
    vtufile.close();


    if(myrank) return;
    std::stringstream pvtufname;
    pvtufname<<fname<<".pvtp";
    std::ofstream pvtufile;
    pvtufile.open(pvtufname.str().c_str());
    if(pvtufile.fail()) return;
    pvtufile<<"<?xml version=\"1.0\"?>\n";
    pvtufile<<"<VTKFile type=\"PPolyData\">\n";
    pvtufile<<"  <PPolyData GhostLevel=\"0\">\n";
    pvtufile<<"      <PPoints>\n";
    pvtufile<<"        <PDataArray type=\"Float"<<sizeof(VTKReal)*8<<"\" NumberOfComponents=\""<<COORD_DIM<<"\" Name=\"Position\"/>\n";
    pvtufile<<"      </PPoints>\n";
    if(value.size()){ // value
      pvtufile<<"      <PPointData>\n";
      pvtufile<<"        <PDataArray type=\"Float"<<sizeof(VTKReal)*8<<"\" NumberOfComponents=\""<<value.size()/pt_cnt<<"\" Name=\""<<"value"<<"\"/>\n";
      pvtufile<<"      </PPointData>\n";
    }
    {
      // Extract filename from path.
      std::stringstream vtupath;
      vtupath<<'/'<<fname;
      std::string pathname = vtupath.str();
      auto found = pathname.find_last_of("/\\");
      std::string fname_ = pathname.substr(found+1);
      for(sctl::Integer i=0;i<np;i++) pvtufile<<"      <Piece Source=\""<<fname_<<"_"<<std::setfill('0')<<std::setw(6)<<i<<".vtp\"/>\n";
    }
    pvtufile<<"  </PPolyData>\n";
    pvtufile<<"</VTKFile>\n";
    pvtufile.close();
  }
};
struct VTUData {
  typedef float VTKReal;

  // Point data
  sctl::Vector<VTKReal> coord;  // always 3D
  sctl::Vector<VTKReal> value;

  // Cell data
  sctl::Vector<int32_t> connect;
  sctl::Vector<int32_t> offset;
  sctl::Vector<uint8_t> types;

  void WriteVTK(const std::string& fname, const sctl::Comm& comm = sctl::Comm::Self()) const {
    typedef typename VTUData::VTKReal VTKReal;
    sctl::Long value_dof = 0;
    {  // Write vtu file.
      std::ofstream vtufile;
      {  // Open file for writing.
        std::stringstream vtufname;
        vtufname << fname << std::setfill('0') << std::setw(6) << comm.Rank() << ".vtu";
        vtufile.open(vtufname.str().c_str());
        if (vtufile.fail()) return;
      }
      {  // Write to file.
        sctl::Long pt_cnt = coord.Dim() / 3;
        sctl::Long cell_cnt = types.Dim();
        value_dof = (pt_cnt ? value.Dim() / pt_cnt : 0);

        sctl::Vector<int32_t> mpi_rank;
        {  // Set  mpi_rank
          sctl::Integer new_myrank = comm.Rank();
          mpi_rank.ReInit(pt_cnt);
          for (sctl::Long i = 0; i < mpi_rank.Dim(); i++) mpi_rank[i] = new_myrank;
        }

        bool isLittleEndian;
        {  // Set isLittleEndian
          uint16_t number = 0x1;
          uint8_t *numPtr = (uint8_t *)&number;
          isLittleEndian = (numPtr[0] == 1);
        }

        sctl::Long data_size = 0;
        vtufile << "<?xml version=\"1.0\"?>\n";
        vtufile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"" << (isLittleEndian ? "LittleEndian" : "BigEndian") << "\">\n";
        // ===========================================================================
        vtufile << "  <UnstructuredGrid>\n";
        vtufile << "    <Piece NumberOfPoints=\"" << pt_cnt << "\" NumberOfCells=\"" << cell_cnt << "\">\n";
        //---------------------------------------------------------------------------
        vtufile << "      <Points>\n";
        vtufile << "        <DataArray type=\"Float" << sizeof(VTKReal) * 8 << "\" NumberOfComponents=\"3\" Name=\"Position\" format=\"appended\" offset=\"" << data_size << "\" />\n";
        data_size += sizeof(uint32_t) + coord.Dim() * sizeof(VTKReal);
        vtufile << "      </Points>\n";
        //---------------------------------------------------------------------------
        vtufile << "      <PointData>\n";
        if (value_dof) {  // value
          vtufile << "        <DataArray type=\"Float" << sizeof(VTKReal) * 8 << "\" NumberOfComponents=\"" << value_dof << "\" Name=\"value\" format=\"appended\" offset=\"" << data_size << "\" />\n";
          data_size += sizeof(uint32_t) + value.Dim() * sizeof(VTKReal);
        }
        {  // mpi_rank
          vtufile << "        <DataArray type=\"Int32\" NumberOfComponents=\"1\" Name=\"mpi_rank\" format=\"appended\" offset=\"" << data_size << "\" />\n";
          data_size += sizeof(uint32_t) + pt_cnt * sizeof(int32_t);
        }
        vtufile << "      </PointData>\n";
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        vtufile << "      <Cells>\n";
        vtufile << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"appended\" offset=\"" << data_size << "\" />\n";
        data_size += sizeof(uint32_t) + connect.Dim() * sizeof(int32_t);
        vtufile << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"appended\" offset=\"" << data_size << "\" />\n";
        data_size += sizeof(uint32_t) + offset.Dim() * sizeof(int32_t);
        vtufile << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"appended\" offset=\"" << data_size << "\" />\n";
        data_size += sizeof(uint32_t) + types.Dim() * sizeof(uint8_t);
        vtufile << "      </Cells>\n";
        //---------------------------------------------------------------------------
        vtufile << "    </Piece>\n";
        vtufile << "  </UnstructuredGrid>\n";
        // ===========================================================================
        vtufile << "  <AppendedData encoding=\"raw\">\n";
        vtufile << "    _";

        int32_t block_size;
        {  // coord
          block_size = coord.Dim() * sizeof(VTKReal);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (coord.Dim()) vtufile.write((char *)&coord[0], coord.Dim() * sizeof(VTKReal));
        }
        if (value_dof) {  // value
          block_size = value.Dim() * sizeof(VTKReal);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (value.Dim()) vtufile.write((char *)&value[0], value.Dim() * sizeof(VTKReal));
        }
        {  // mpi_rank
          block_size = mpi_rank.Dim() * sizeof(int32_t);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (mpi_rank.Dim()) vtufile.write((char *)&mpi_rank[0], mpi_rank.Dim() * sizeof(int32_t));
        }
        {  // block_size
          block_size = connect.Dim() * sizeof(int32_t);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (connect.Dim()) vtufile.write((char *)&connect[0], connect.Dim() * sizeof(int32_t));
        }
        {  // offset
          block_size = offset.Dim() * sizeof(int32_t);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (offset.Dim()) vtufile.write((char *)&offset[0], offset.Dim() * sizeof(int32_t));
        }
        {  // types
          block_size = types.Dim() * sizeof(uint8_t);
          vtufile.write((char *)&block_size, sizeof(int32_t));
          if (types.Dim()) vtufile.write((char *)&types[0], types.Dim() * sizeof(uint8_t));
        }

        vtufile << "\n";
        vtufile << "  </AppendedData>\n";
        // ===========================================================================
        vtufile << "</VTKFile>\n";
      }
      vtufile.close();  // close file
    }
    if (!comm.Rank()) {  // Write pvtu file
      std::ofstream pvtufile;
      {  // Open file for writing
        std::stringstream pvtufname;
        pvtufname << fname << ".pvtu";
        pvtufile.open(pvtufname.str().c_str());
        if (pvtufile.fail()) return;
      }
      {  // Write to file.
        pvtufile << "<?xml version=\"1.0\"?>\n";
        pvtufile << "<VTKFile type=\"PUnstructuredGrid\">\n";
        pvtufile << "  <PUnstructuredGrid GhostLevel=\"0\">\n";
        pvtufile << "      <PPoints>\n";
        pvtufile << "        <PDataArray type=\"Float" << sizeof(VTKReal) * 8 << "\" NumberOfComponents=\"3\" Name=\"Position\"/>\n";
        pvtufile << "      </PPoints>\n";
        pvtufile << "      <PPointData>\n";
        if (value_dof) {  // value
          pvtufile << "        <PDataArray type=\"Float" << sizeof(VTKReal) * 8 << "\" NumberOfComponents=\"" << value_dof << "\" Name=\"value\"/>\n";
        }
        {  // mpi_rank
          pvtufile << "        <PDataArray type=\"Int32\" NumberOfComponents=\"1\" Name=\"mpi_rank\"/>\n";
        }
        pvtufile << "      </PPointData>\n";
        {
          // Extract filename from path.
          std::stringstream vtupath;
          vtupath << '/' << fname;
          std::string pathname = vtupath.str();
          std::string fname_ = pathname.substr(pathname.find_last_of("/\\") + 1);
          // char *fname_ = (char*)strrchr(vtupath.str().c_str(), '/') + 1;
          // std::string fname_ =
          // boost::filesystem::path(fname).filename().string().
          for (sctl::Integer i = 0; i < comm.Size(); i++) pvtufile << "      <Piece Source=\"" << fname_ << std::setfill('0') << std::setw(6) << i << ".vtu\"/>\n";
        }
        pvtufile << "  </PUnstructuredGrid>\n";
        pvtufile << "</VTKFile>\n";
      }
      pvtufile.close();  // close file
    }
  };
};

template <class Real> void WriteVTK(const char* fname, const sctl::Vector<Surface<Real>>& Svec, const sctl::Vector<Real> F, const sctl::Comm& comm) {
  VTKData data;
  typedef VTKData::VTKReal VTKReal;
  auto& point_coord =data.point_coord ;
  auto& point_value =data.point_value ;
  auto& poly_connect=data.poly_connect;
  auto& poly_offset =data.poly_offset ;
  constexpr sctl::Integer COORD_DIM = VTKData::COORD_DIM;

  sctl::Long dof, offset = 0;
  { // Set dof
    sctl::Long Npt = 0;
    for (const auto& S : Svec) Npt += S.NTor() * S.NPol();
    dof = F.Dim() / Npt;
    SCTL_ASSERT(F.Dim() == dof * Npt);
  }
  for (auto& S: Svec) { // Set point_coord, point_value, poly_connect
    sctl::Long Nt = S.NTor();
    sctl::Long Np = S.NPol();
    sctl::Long N = Nt * Np;

    for (sctl::Long i = 0; i < Nt; i++) {
      for (sctl::Long j = 0; j < Np; j++) {
        sctl::Long i0 = (i + 0) % Nt;
        sctl::Long i1 = (i + 1) % Nt;
        sctl::Long j0 = (j + 0) % Np;
        sctl::Long j1 = (j + 1) % Np;

        poly_connect.push_back(point_coord.size() / COORD_DIM + Np*i0+j0);
        poly_connect.push_back(point_coord.size() / COORD_DIM + Np*i1+j0);
        poly_connect.push_back(point_coord.size() / COORD_DIM + Np*i1+j1);
        poly_connect.push_back(point_coord.size() / COORD_DIM + Np*i0+j1);
        poly_offset.push_back(poly_connect.size());
      }
    }

    const auto X = S.Coord();
    SCTL_ASSERT(X.Dim() == COORD_DIM * N);
    for (sctl::Long i = 0; i < N; i++){ // Set point_coord
      for (sctl::Integer k = 0; k < COORD_DIM; k++) point_coord.push_back((VTKReal)X[k * N + i]);
    }

    for (sctl::Long i = 0; i < N; i++){ // Set point_value
      for (sctl::Long k = 0; k < dof; k++) point_value.push_back((VTKReal)F[dof * offset + k * N + i]);
    }
    offset += N;
  }
  data.WriteVTK(fname, comm);
}

template <class Real> void WriteVTK(const char* fname, const Surface<Real>& S, const sctl::Vector<Real> F, const sctl::Comm& comm) {
  WriteVTK(fname, sctl::Vector<Surface<Real>>(1,sctl::Ptr2Itr<Surface<Real>>((Surface<Real>*)&S,1),false), F, comm);
}

template <class Real> void WriteVTK(const std::string& fname, const sctl::Integer NFP, bool half_period, const sctl::Long surf_Nt, const sctl::Long surf_Np, const sctl::Vector<Real>& X, const sctl::Long src_Nt, const sctl::Long src_Np, const sctl::Vector<Real>& F) {
  sctl::Vector<Real> X_, F_;
  sctl::Vector<biest::Surface<Real>> Svec(1);
  Svec[0] = biest::Surface<Real>(NFP*(half_period?2:1)*src_Nt, src_Np, biest::SurfType::None);
  SurfaceOp<Real>::CompleteVecField(X_, true, half_period, NFP, surf_Nt, surf_Np, X, (half_period?sctl::const_pi<Real>()/(NFP*src_Nt*2):0)-(half_period?sctl::const_pi<Real>()/(NFP*surf_Nt*2):0));
  SurfaceOp<Real>::Resample(Svec[0].Coord(), NFP*(half_period?2:1)*src_Nt, src_Np, X_, NFP*(half_period?2:1)*surf_Nt, surf_Np);

  SurfaceOp<Real>::CompleteVecField(F_, false, half_period, NFP, src_Nt, src_Np, F);
  biest::WriteVTK(fname.c_str(), Svec, F_);
}

}
