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

#include "rhotabularThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(rhotabularThermo, 0);
    defineRunTimeSelectionTable(rhotabularThermo, fvMesh);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::rhotabularThermo::rhotabularThermo(const fvMesh& mesh, const word& phaseName)
:
    fluidThermo(mesh, phaseName),

    rho_
     (
         IOobject
         (
             phasePropertyName("thermo:rho"),
             mesh.time().timeName(),
             mesh,
             IOobject::READ_IF_PRESENT,
             IOobject::AUTO_WRITE
         ),
         mesh,
         dimDensity
     ),

    psi_
    (
        IOobject
        (
            phasePropertyName("thermo:psi"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionSet(0, -2, 2, 0, 0)
    ),

    mu_
    (
        IOobject
        (
            phasePropertyName("thermo:mu"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionSet(1, -1, -1, 0, 0)
    ),
    densityTable("constant/densityTable")
{
    densityTable.outOfBounds(interpolation2DTable<scalar>::CLAMP);
}


Foam::rhotabularThermo::rhotabularThermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    fluidThermo(mesh, dict, phaseName),
    rho_
    (
        IOobject
        (
            phasePropertyName("thermo:rho"),
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimDensity
    ),

    psi_
    (
        IOobject
        (
            phasePropertyName("thermo:psi"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionSet(0, -2, 2, 0, 0)
    ),

    mu_
    (
        IOobject
        (
            phasePropertyName("thermo:mu"),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionSet(1, -1, -1, 0, 0)
    )
{}
// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::rhotabularThermo> Foam::rhotabularThermo::New
(
    const fvMesh& mesh,
    const word& phaseName
)
{
    return basicThermo::New<rhotabularThermo>(mesh, phaseName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::rhotabularThermo::~rhotabularThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::volScalarField& Foam::rhotabularThermo::lookupOrConstruct2
(
    const fvMesh& mesh,
    const char* name,
    dimensionSet units
) const
{
    if (!mesh.objectRegistry::foundObject<volScalarField>(name))//大改动
    {
        volScalarField* fPtr
        (
            new volScalarField
            (
                IOobject
                (
                    name,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
		dimensionedScalar("tmp", units, 0)
            )
        );

        // Transfer ownership of this object to the objectRegistry
        fPtr->store(fPtr);
    }

    return const_cast<volScalarField&>
    (
        mesh.objectRegistry::lookupObject<volScalarField>(name)
    );
}

Foam::tmp<Foam::volScalarField> Foam::rhotabularThermo::rho() const//大改动
{
    volScalarField rho_ =
	lookupOrConstruct2
	(
	    T_.mesh(),
	    phasePropertyName("thermo:rho_").c_str(),
	    dimensionSet(1, -1, -2, 0, 0)
	 );
    volScalarField T_back =
	lookupOrConstruct2
	(
	    T_.mesh(),
	    phasePropertyName("thermo:T_back").c_str(),
	    dimensionSet(0, 0, 0, 1, 0)
	 );

    volScalarField p_back =
	lookupOrConstruct2
	(
	    T_.mesh(),
	    phasePropertyName("thermo:p_back").c_str(),
	    dimensionSet(1, -3, 0, 0, 0)
	 );
    /* Check if the temperature or pressure fields
       have changed since the last iteration. */
    forAll(T_, faceI)
    {
	if(T_[faceI] != T_back[faceI] || p_[faceI] != p_back[faceI])
	{
		//Info << "difference" << endl;
    		forAll(T_, faceI2)
		{
			rho_[faceI2] = densityTable(T_[faceI2], p_[faceI2]);
			T_back[faceI2] = T_[faceI2];
			p_back[faceI2] = p_[faceI2];
		}
		return rho_;
	}
    }
    return rho_;
}

Foam::tmp<Foam::scalarField> Foam::rhotabularThermo::rho(const label patchi) const
{
    return rho_.boundaryField()[patchi];
}

Foam::volScalarField& Foam::rhotabularThermo::rho()
{
    return rho_;
}

void Foam::rhotabularThermo::correctRho(const Foam::volScalarField& deltaRho)
{
    rho_ += deltaRho;
}
//void Foam::rhotabularThermo::correctRho(const Foam::volScalarField& deltaRho)
//{}


const Foam::volScalarField& Foam::rhotabularThermo::psi() const
{
    return rho()/(p_ + dimensionedScalar("tmpstabs", p_.dimensions(), SMALL));
}

/*const Foam::volScalarField& Foam::rhotabularThermo::psi() const
{
    return psi_;
}*/

Foam::tmp<Foam::volScalarField> Foam::rhotabularThermo::mu() const
{
    return mu_;
}


Foam::tmp<Foam::scalarField> Foam::rhotabularThermo::mu(const label patchi) const
{
    return mu_.boundaryField()[patchi];
}


// ************************************************************************* //
