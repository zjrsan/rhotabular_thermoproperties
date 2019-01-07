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

Class
    Foam::rhotabularThermo

Description
    Basic thermodynamic properties based on compressibility

SourceFiles
    rhotabularThermo.C

\*---------------------------------------------------------------------------*/

#ifndef rhotabularThermo_H
#define rhotabularThermo_H

#include "fluidThermo.H"
#include "rhoThermo.H"
#include "runTimeSelectionTables.H"
#include "interpolation2DTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class rhotabularThermo Declaration
\*---------------------------------------------------------------------------*/

class rhotabularThermo
:
    public fluidThermo
{

protected:

    // Protected data
        //- Density field [kg/m^3]
        volScalarField rho_;

        //- Compressibility [s^2/m^2]
       volScalarField psi_;

        //- Dynamic viscosity [kg/m/s]
        volScalarField mu_;

        //- Density [kg/m^3]
        interpolation2DTable<scalar> densityTable;

    // Protected Member Functions

        //- Construct as copy (not implemented)
        rhotabularThermo(const rhotabularThermo&);


public:
    //- Runtime type information
    TypeName("rhotabularThermo");


    //- Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        rhotabularThermo,
        fvMesh,
        (const fvMesh& mesh, const word& phaseName),
        (mesh, phaseName)
    );


    // Constructors
        //- Construct from mesh and phase name
        rhotabularThermo
        (
	    const fvMesh&,
            const word& phaseName
	);
        rhotabularThermo
        (
            const fvMesh&,
            const dictionary&,
            const word& phaseName
        );

    //- Selector
    static autoPtr<rhotabularThermo> New
    (
        const fvMesh& mesh,
        const word& phaseName=word::null
    );


    //- Destructor
    virtual ~rhotabularThermo();


    // Member functions

        volScalarField& lookupOrConstruct2
        (
            const fvMesh& mesh,
            const char* name,
	    dimensionSet units
        ) const;

        // Fields derived from thermodynamic state variables

            //- Add the given density correction to the density field.
            //  Used to update the density field following pressure solution.
            //  For psiThermo does nothing.
            //- Density [kg/m^3]
            virtual tmp<volScalarField> rho() const;

            //- Density for patch [kg/m^3]
            virtual tmp<scalarField> rho(const label patchi) const;

            //- Return non-const access to the local density field [kg/m^3]
            virtual volScalarField& rho();

            //- Add the given density correction to the density field.
            //  Used to update the density field following pressure solution
            virtual void correctRho(const volScalarField& deltaRho);

            //- Compressibility [s^2/m^2]
            virtual const volScalarField& psi() const;


        // Access to transport state variables

            //- Dynamic viscosity of mixture [kg/m/s]
            virtual tmp<volScalarField> mu() const;

            //- Dynamic viscosity of mixture for patch [kg/m/s]
            virtual tmp<scalarField> mu(const label patchi) const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
