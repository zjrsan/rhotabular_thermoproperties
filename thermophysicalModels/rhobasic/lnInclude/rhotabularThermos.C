/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2018 Yuusha, tilasoldo and cbunge
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of tilasoldo and Yuusha contribution to OpenFOAM.
    It is based on chriss85 contribution for OpenFOAM 2.3.x.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "rhoThermo.H"
#include "psiThermo.H"
#include "makeThermo.H"

#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/specie/specie.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/equationOfState/tabularEOS/tabularEOS.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/thermo/hTabular/hTabularThermo.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/thermo/sensibleEnthalpy/sensibleEnthalpy.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/thermo/sensibleInternalEnergy/sensibleInternalEnergy.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/thermo/thermo/thermo.H"

#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhospecie/transport/tabular/tabularTransport.H"

//#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhobasic/rhoThermo/heRhoThermo.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhobasic/rhotabularThermo/heRhoTabularThermo.H"
#include "/home/dyfluid/OpenFOAM/dyfluid-5.x/src/src_rhotabularnew/thermophysicalModels/rhobasic/mixtures/pureMixture/pureMixture.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/* * * * * * * * * * * * * * Enthalpy-based * * * * * * * * * * * * * */

makeThermo
(
    rhoThermo,
    heRhoTabularThermo,
    pureMixture,
    tabularTransport,
    sensibleEnthalpy,
    hTabularThermo,
    tabularEOS,
    specie
);

/*makeThermo
(
    psiThermo,
    heRhoTabularThermo,
    pureMixture,
    tabularTransport,
    sensibleInternalEnergy,
    hTabularThermo,
    tabularEOS,
    specie
);*/

}
/* * * * * * * * * * * * * * Internal-energy-based * * * * * * * * * * * * * */
// This uses two tables for e(p, T) and T (e, p) NOTE: Ensure the correct table (internalEnergy or Enthalpy) is being referened in run/constant/thermophysicalProperties file.
/*makeThermo
(
    psiThermo,
    heTabularThermo,
    pureMixture,
    tabularTransport,
    sensibleInternalEnergy,
    hTabularThermo,
    tabularEOS,
    specie
);*/
 
// This uses two tables for h(p, T) and T(h, p) NOTE: Ensure the correct table (internalEnergy or Enthalpy) is being referened in run/constant/thermophysicalProperties file.
//makeThermo
//(
//    psiThermo,
//    hePsiThermo,
//    pureMixture,
//    tabularTransport,
//    sensibleEnthalpy,
//    hTabularThermo,
//    tabularEOS,
//    specie
//);





   
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// End namespace Foam

// ************************************************************************* //
