Cantera Research Fork: Stretched Thickened-Flame Model
======================================================

Overview
--------

This repository is a research fork of Cantera implementing additional modelling
capabilities for one-dimensional laminar flame calculations, with a focus on
simplified transport properties and stretched thickened-flame modelling.

The ``feature/stretched-thickened-flame`` branch includes:

* A simplified transport model with user-controlled effective Lewis and
  Prandtl numbers.
* An artificially thickened-flame model for one-dimensional freely propagating
  and strained flame calculations.
* A stretched thickened-flame model intended to preserve the response of an
  artificially thickened flame to stretch.
* Python frontend access to the added model controls.
* Validation notebooks comparing the implementations against reference
  calculations and published results.

This fork is experimental research software developed as part of a PhD project.
It is not an official Cantera release.

Relationship to Cantera
-----------------------

Cantera is an open-source suite of tools for chemical kinetics,
thermodynamics, transport processes, and reacting-flow simulations.

This repository is based on Cantera and retains the original Cantera licence
and attribution.

* `Cantera website <https://cantera.org>`_
* `Original Cantera source code <https://github.com/Cantera/cantera>`_
* `Cantera documentation <https://cantera.org/documentation>`_

Fork Lineage
~~~~~~~~~~~~

The stretched thickened-flame implementation was developed from the author's
existing Cantera research fork:

* `Base fork <https://github.com/cilliant479/cantera-tcd/tree/main>`_
* Base version: Cantera ``4.0.0a1``
* `Feature branch <https://github.com/cilliant479/cantera-tcd/tree/feature/stretched-thickened-flame>`_

The ``main`` branch of ``cantera-tcd`` provides the baseline fork used for this
work. The ``feature/stretched-thickened-flame`` branch adds the simplified
transport, thickened-flame, and stretched thickened-flame functionality
described here.

Implemented Models
------------------

Simplified Transport Model
~~~~~~~~~~~~~~~~~~~~~~~~~~

The simplified transport model allows the user to prescribe effective Lewis
and Prandtl numbers for controlled one-dimensional flame calculations.

A common effective mass diffusivity is assigned to all species, removing
preferential diffusion and providing a controlled transport framework for
examining thickened-flame behaviour.

Artificially Thickened-Flame Model
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The artificially thickened-flame model of Colin et al. [1]_ increases the flame
thickness by modifying the balance between diffusion and chemical source terms
while preserving the unstretched laminar flame speed.

The implementation is tested using one-dimensional freely propagating flames,
for which the expected changes in flame speed, flame thickness, and temperature
profile can be examined directly.

Stretched Thickened-Flame Model
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The formulation of Detomaso et al. [2]_ generalises the classical
thickened-flame transformation to stretched flames.

The model introduces separate factors for thermal conduction, species
diffusion, and chemical source terms:

* ``Fth``: thermal-conduction factor.
* ``Fsp``: species-diffusion factor.
* ``Fr``: reaction-rate factor.

These factors are supplied through the Python frontend. The validation scripts
use the relationships proposed by Detomaso et al., but alternative
relationships may also be tested without recompiling the C++ backend.

Documentation
-------------

An overview of the technical documentation is available at:

* `Documentation overview <docs/README.md>`_
* `Full PDF documentation <docs/cantera_tcd_documentation.pdf>`_

The documentation contains:

* The simplified transport formulation.
* The governing one-dimensional flame equations.
* Backend modifications to the species and energy equations.
* Python frontend access to the new controls.
* Artificially thickened-flame validation.
* Stretched thickened-flame validation.
* Comparisons with published reference values.
* Current limitations of the implementation.

Installation and Build from Source
----------------------------------

This branch must currently be built from source.

The workflow described below uses a local Python virtual environment named
``ct-env`` and a helper script, ``dev_env.sh``, to configure the local build
paths.

The workflow has been tested on:

* Ubuntu 24.04.4 LTS.
* Windows 10 using WSL with Ubuntu 24.04.1 LTS.

The instructions may require adaptation for other operating systems, compiler
versions, Python versions, or SCons configurations.

At present, ``scons install`` is not used for this fork. The locally compiled
Python package and shared library are loaded directly from the ``build``
directory.

1. Install System Dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On Ubuntu, install the required compiler and development packages:

.. code-block:: bash

   sudo apt update
   sudo apt install build-essential g++ git python3 python3-venv \
       python3-dev doxygen libboost-dev

If compilation reports that ``Python.h`` cannot be found, install the
development package corresponding to the active Python version. For example:

.. code-block:: bash

   sudo apt install python3.12-dev

Additional system dependencies may be required depending on the selected
Cantera build configuration.

2. Clone the Repository
~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   git clone https://github.com/cilliant479/cantera-tcd.git
   cd cantera-tcd
   git checkout feature/stretched-thickened-flame

3. Create a Python Virtual Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create and activate a local virtual environment named ``ct-env``:

.. code-block:: bash

   python3 -m venv ct-env
   source ct-env/bin/activate

Upgrade the Python packaging tools:

.. code-block:: bash

   python -m pip install --upgrade pip setuptools wheel

Install the Python build, testing, and validation dependencies:

.. code-block:: bash

   python -m pip install scons cython numpy scipy matplotlib pandas jupyter
   python -m pip install ruamel.yaml pytest jinja2
   python -m pip install "typing_extensions>=4.13.0"

The ``ct-env`` directory is a local development environment and should not be
committed to the repository.

4. Load the Development Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The repository includes a helper script named ``dev_env.sh``.

The script should be sourced rather than executed:

.. code-block:: bash

   source dev_env.sh

The script:

* Activates ``ct-env``.
* Adds ``build/python`` to ``PYTHONPATH``.
* Adds ``build/lib`` to ``LD_LIBRARY_PATH``.
* Sets ``CANTERA_DATA`` to the repository's ``data`` directory.

The current script is:

.. code-block:: bash

   #!/usr/bin/env bash

   # This file should be sourced, not executed:
   #   source dev_env.sh

   REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

   cd "$REPO_ROOT" || return 1

   source ct-env/bin/activate

   export PYTHONPATH="$REPO_ROOT/build/python:$PYTHONPATH"
   export LD_LIBRARY_PATH="$REPO_ROOT/build/lib:$LD_LIBRARY_PATH"
   export CANTERA_DATA="$REPO_ROOT/data"

   echo "Cantera dev environment loaded."
   echo "Repo: $REPO_ROOT"
   echo "Python: $(which python)"

5. Build Cantera
~~~~~~~~~~~~~~~~

Clean files from any previous build:

.. code-block:: bash

   scons clean

Build the fork using SCons:

.. code-block:: bash

   scons build -j8

The value supplied to ``-j`` may be adjusted for the number of processor cores
available on the machine.

The local build is created in:

.. code-block:: text

   build/python
   build/lib

6. Verify the Local Build
~~~~~~~~~~~~~~~~~~~~~~~~~

After building and sourcing ``dev_env.sh``, verify that Python imports the
modified local version:

.. code-block:: python

   import cantera as ct

   print(ct.__version__)
   print(ct.__file__)

The expected version is:

.. code-block:: text

   4.0.0a1

The path printed by ``ct.__file__`` should point to the local repository,
normally within ``build/python``.

If Python imports Cantera from a global installation, Conda environment, or
``site-packages``, the local development environment has not been configured
correctly.

Usage
-----

A minimal freely propagating flame example is shown below.

.. code-block:: python

   import cantera as ct

   gas = ct.Solution("gri30.yaml")

   gas.TP = 300.0, ct.one_atm
   gas.set_equivalence_ratio(
       phi=0.7,
       fuel="CH4",
       oxidizer={"O2": 1.0, "N2": 3.76},
   )

   gas.transport_model = "simplified"
   gas.set_lewis_number(1.4)
   gas.set_prandtl_number(0.7)

   flame = ct.FreeFlame(gas, width=0.03)

   # Arguments: Fth, Fsp, Fr
   flame.set_stretched_thickened_flame(
       1.0,
       1.0,
       1.0,
   )

   flame.solve(
       loglevel=1,
       auto=True,
   )

   print("Laminar flame speed:", flame.velocity[0])

The original unthickened equations are recovered using:

.. code-block:: python

   flame.set_stretched_thickened_flame(
       1.0,
       1.0,
       1.0,
   )

A classical thickened-flame transformation can be applied using equal factors:

.. code-block:: python

   F = 5.0

   flame.set_stretched_thickened_flame(
       F,
       F,
       F,
   )

For the stretched thickened-flame formulation, ``Fth``, ``Fsp``, and ``Fr``
may be assigned independently:

.. code-block:: python

   flame.set_stretched_thickened_flame(
       Fth,
       Fsp,
       Fr,
   )

The assigned values can be retrieved using:

.. code-block:: python

   Fth, Fsp, Fr = flame.get_stretched_thickened_flame()

Validation Notebooks
--------------------

The following notebooks demonstrate and validate the implemented models:

* `Simplified transport and ATF validation
  <samples/python/stretchedTF/atf_simplified_validation.ipynb>`_
* `Stretched thickened-flame validation
  <samples/python/stretchedTF/stf_validation.ipynb>`_

Run Jupyter from the configured development environment:

.. code-block:: bash

   source dev_env.sh
   jupyter notebook

The notebooks should be run from top to bottom using the locally compiled
Cantera fork.

Validation Scope
----------------

The simplified transport and thickened-flame implementations have been tested
using the one-dimensional cases included in the validation notebooks.

The stretched thickened-flame factors obtained using the supplied validation
procedure are compared with the published values of Detomaso et al. [2]_.

The implementation is considered validated for the one-dimensional test cases
examined in the documentation. Further validation should be performed before
applying the implementation to new fuels, chemical mechanisms, configurations,
or large-eddy simulations.

Implementation Notes
--------------------

The implementation modifies backend and frontend components of Cantera.

Backend changes include:

* A simplified transport class.
* Modifications to species-diffusion terms.
* Modifications to thermal-conduction terms.
* Modifications to chemical source terms.
* Storage and assignment of ``Fth``, ``Fsp``, and ``Fr``.

Frontend changes expose the added transport and thickened-flame controls through
the Python interface.

A more detailed description of the modified equations and source files is
provided in the technical documentation.

Limitations
-----------

This branch is intended for research use and should be treated as experimental.

Current limitations include:

* Validation is limited to the cases supplied in the validation notebooks.
* The implementation is focused on one-dimensional laminar flame calculations.
* The model has not been tested with every Cantera transport model, chemical
  mechanism, or flame configuration.
* The thickening factors are currently supplied as global scalar values.
* The installation workflow uses the local build because ``scons install`` is
  not currently used.
* The API may change as the implementation is developed further.

Users should verify the implementation carefully before applying it to new
mechanisms, fuels, configurations, or production calculations.

Citation and Attribution
------------------------

Users of this fork should cite:

#. The original Cantera software package.
#. The artificially thickened-flame model of Colin et al. [1]_.
#. The stretched thickened-flame model of Detomaso et al. [2]_.
#. This research fork.

When citing this fork, report the repository URL, branch or release tag, exact
commit hash used, and access date.

Suggested citation:

   Thomas, C. *Cantera Research Fork: Stretched Thickened-Flame Model*.
   GitHub repository, branch ``feature/stretched-thickened-flame``.
   Available at
   https://github.com/cilliant479/cantera-tcd/tree/feature/stretched-thickened-flame.
   Include the release tag or exact commit hash used and the date accessed.

To obtain the exact commit hash for a local checkout, run:

.. code-block:: bash

   git rev-parse HEAD

Because this fork is based on a Cantera development version, users should also
state that the implementation is based on Cantera ``4.0.0a1``.

Licence
-------

This repository retains the original Cantera licence and attribution.

The added stretched thickened-flame implementation is provided as experimental
research software without warranty. See the repository licence file for
details.

Development Status
------------------

Current status:

* Simplified transport model: tested in a validation notebook.
* Artificially thickened-flame model: tested in a validation notebook.
* Stretched thickened-flame model: compared with published reference values.
* Local source build: working using ``scons build``.
* Installation using ``scons install``: not currently used.

Contact
-------

Author: `Cillian Thomas <mailto:thomasci@tcd.ie>`_

This fork was developed as part of a PhD research project involving combustion
modelling, large-eddy simulation, flame dynamics, and artificially thickened
flame models.

References
----------

.. [1] Colin, O., Ducros, F., Veynante, D., and Poinsot, T.
   "A thickened flame model for large eddy simulations of turbulent premixed
   combustion." *Physics of Fluids*, 12(7), 1843--1863, 2000.
   https://doi.org/10.1063/1.870436

.. [2] Detomaso, N., Hok, J.-J., Dounia, O., Laera, D., and Poinsot, T.
   "A generalization of the Thickened Flame model for stretched flames."
   *Combustion and Flame*, 258, 113080, 2023.
   https://doi.org/10.1016/j.combustflame.2023.113080
