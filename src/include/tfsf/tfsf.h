#ifndef _TFSF_H_
#define _TFSF_H_

#include <memory>

#include "shape/cube.h"
#include "util/type_define.h"
#include "waveform/waveform.h"
namespace xfdtd {
class TFSF {
 public:
  TFSF(SpatialIndex distance_x, SpatialIndex distance_y,
       SpatialIndex distance_z, double theta_inc, double phi_inc,
       double e_theta, double e_phi, std::unique_ptr<Waveform> waveform);
  TFSF(TFSF &&ohter) = default;
  virtual ~TFSF() = default;

  struct TFSFBoundaryIndex {
    SpatialIndex start_x;
    SpatialIndex start_y;
    SpatialIndex start_z;
    SpatialIndex nx;
    SpatialIndex ny;
    SpatialIndex nz;
  };

  inline auto getWaveform() { return _waveform->clone(); }
  inline std::tuple<SpatialIndex, SpatialIndex, SpatialIndex> getDistance()
      const {
    return {_distance_x, _distance_y, _distance_z};
  }
  inline double getIncidentTheta() const { return _theta_inc; }
  inline double getIncidentPhi() const { return _phi_inc; }
  inline double getETehta() const { return _e_theta; }
  inline double getEPhi() const { return _e_phi; }
  inline double getIncidentFieldWaveformValueByTime(double time) {
    return _waveform->getValueByTime(time);
  }
  inline double getDt() const { return _dt; }
  inline double getDx() const { return _dx; }
  inline double getDy() const { return _dy; }
  inline double getDz() const { return _dz; }
  inline PointVector getKVector() const { return _k; }
  inline double getL0() const { return _l_0; }
  inline SpatialIndex getStartIndexX() const {
    return _tfsf_boundary_index.start_x;
  }
  inline SpatialIndex getEndIndexX() const {
    return getStartIndexX() + getNx();
  }
  inline SpatialIndex getStartIndexY() const {
    return _tfsf_boundary_index.start_y;
  }
  inline SpatialIndex getEndIndexY() const {
    return getStartIndexX() + getNx();
  }
  inline SpatialIndex getStartIndexZ() const {
    return _tfsf_boundary_index.start_z;
  }
  inline SpatialIndex getEndIndexZ() const {
    return getStartIndexZ() + getNz();
  }
  inline SpatialIndex getNx() const { return _tfsf_boundary_index.nx; }
  inline SpatialIndex getNy() const { return _tfsf_boundary_index.ny; }
  inline SpatialIndex getNz() const { return _tfsf_boundary_index.nz; }
  inline const Cube *getTFSFCubeBox() const {
    if (_tfsf_box == nullptr) {
      throw std::runtime_error("TFSF box is not initialized");
    }
    return _tfsf_box.get();
  }

  virtual void init(const Cube *simulation_box, double dx, double dy, double dz,
                    double dt, TFSFBoundaryIndex tfsf_boundary_index) = 0;
  virtual void updateIncidentField(size_t current_time_step) = 0;
  virtual void updateH() = 0;
  virtual void updateE() = 0;

 protected:
  void initTFSF(const Cube *simulation_box, double dx, double dy, double dz,
                double dt, TFSFBoundaryIndex tfsf_boundary_index);

  virtual void allocateKDotR() = 0;
  virtual void allocateEiHi() = 0;
  virtual void caculateKDotR() = 0;

 private:
  SpatialIndex _distance_x;
  SpatialIndex _distance_y;
  SpatialIndex _distance_z;
  double _theta_inc;
  double _phi_inc;
  double _e_theta;
  double _e_phi;
  PointVector _k;
  std::unique_ptr<Waveform> _waveform;

  double _dt;
  double _dx;
  double _dy;
  double _dz;
  TFSFBoundaryIndex _tfsf_boundary_index;
  // Eigen::Matrix<double, 8, 3> _fdtd_domain_matrix;
  std::unique_ptr<Cube> _tfsf_box;
  double _l_0;
};
}  // namespace xfdtd

#endif  // _TFSF_H_