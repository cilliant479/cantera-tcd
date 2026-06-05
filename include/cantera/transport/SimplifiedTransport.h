/**
 *  @file UnityLewisTransport.h
 *    Headers for the UnityLewisTransport object, which models transport
 *    properties in ideal gas solutions using the unity Lewis number
 *    approximation
 *    (see @ref tranprops and @link Cantera::UnityLewisTransport UnityLewisTransport @endlink) .
 */

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_SIMPLIFIED_H
#define CT_SIMPLIFIED_H

#include "MixTransport.h"
#include "cantera/thermo/ThermoPhase.h"

namespace Cantera
{
//! Class UnityLewisTransport implements the unity Lewis number approximation
//! for the mixture-averaged species diffusion coefficients. Mixture-averaged
//! transport properties for viscosity and thermal conductivity are inherited
//! from the MixTransport class.
//! @ingroup tranprops
class SimplifiedTransport : public MixTransport
{
public:
    SimplifiedTransport() = default;

    string transportModel() const override {
        return "Simplified";
    }

    void setPrandtl(double pr){
        this->m_prandtl=pr;
    }

    void setLe(double le)
    {
        this->m_le = le;
    }
    //! Returns the unity Lewis number approximation based diffusion
    //! coefficients [m²/s].
    /*!
     * Returns the unity Lewis number approximation based diffusion coefficients
     * for a gas, appropriate for calculating the mass averaged diffusive flux
     * with respect to the mass averaged velocity using gradients of the mole
     * fraction.
     *
     * @f[
     *     D^\prime_{km} = \frac{\lambda}{\rho c_p}
     * @f]
     *
     * In order to obtain the expected behavior from a unity Lewis number model,
     * this formulation requires that the correction velocity be computed as
     *
     * @f[
     *     V_c = \sum \frac{W_k}{\overline{W}} D^\prime_{km} \nabla X_k
     * @f]
     *
     * @param[out] d  Vector of diffusion coefficients for each species. length #m_nsp.
     */
    void getMixDiffCoeffs(span<double> d) override {
        checkArraySize("SimplifiedTransport::getMixDiffCoeffs", d.size(), m_nsp);
        double Dm = thermalConductivity() / (m_thermo->density() * m_thermo->cp_mass() * m_le); // Add Le to denom
        for (size_t k = 0; k < m_nsp; k++) {
            d[k] = Dm;
        }
    }

    //! Thermal diffusion is not enabled in the unity Lewis number model.
    /*!
     * @param[out] dt  Thermal diffusion coefficients all set to zero.
     */
    void getThermalDiffCoeffs(span<double> dt) override {
        checkArraySize("SimplifiedTransport::getThermalDiffCoeffs", dt.size(), m_nsp);
        for (size_t k = 0; k < m_nsp; k++) {
            dt[k] = 0.0;
        }
    }

    //! Not implemented for unity Lewis number approximation
    void getMixDiffCoeffsMole(span<double> d) override {
        throw NotImplementedError("SimplifiedTransport::getMixDiffCoeffsMole");
    }

    //! Returns the unity Lewis number approximation based diffusion
    //! coefficients [m²/s].
    /*!
     * These are the coefficients for calculating the diffusive mass fluxes
     * from the species mass fraction gradients, computed as
     *
     * @f[
     *     D_{km} = \frac{\lambda}{\rho c_p}
     * @f]
     *
     * @param[out] d  Vector of diffusion coefficients for each species; length #m_nsp.
     */
    void getMixDiffCoeffsMass(span<double> d) override {
        checkArraySize("SimplifiedTransport::getMixDiffCoeffsMass", d.size(), m_nsp);
        double Dm = thermalConductivity() / (m_thermo->density() * m_thermo->cp_mass()); // Add Le to denom
        for (size_t k = 0; k < m_nsp; k++) {
            d[k] = Dm;
        } 
    }

    double thermalConductivity () override
    {
    update_T();
    update_C();
    
    m_lambda = m_thermo->cp_mass() * viscosity()/m_prandtl; 
    
    return m_lambda;
    }


protected:
    double m_prandtl = 0.7;
    double m_le = 1;
};
}
#endif
