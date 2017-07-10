# TAU_PROF2JASON
produce jason file for performance metrics using TAU profile summary

# Building TAU with ADIOS

1.	Build ADIOS 1.12 as usual  (source available at https://github.com/ornladios/ADIOS)

2.	 Configure and build TAU 2.26.2 as usual, adding the -adios=/path/to/adios/installation flag at the configure step (source available at http://tau.uoregon.edu/tau.tgz)

3.	 Add /path/to/tau/$arch/bin to your PATH/path environment variable  (where “/path/to/tau” is your TAU installation location)

4.	 set the TAU_MAKEFILE to the Makefile that matches your TAU configuration, located in /path/to/tau/$arch/lib/Makefile.tau-*

# Instrumenting using TAU
When linking your executable(s), replace "${ADIOSDIR}/bin/adios_config -l -f” with "tau_cc.sh -tau:showlibs ${ADIOSDIR}/bin/adios_config -l -f” to get both the TAU link flags/libraries as well as the ADIOS flags and libraries.  If replacing in a Makefile, it might look something like this: "$(shell tau_cc.sh -tau:showlibs) $(shell ${ADIOSDIR}/bin/adios_config -l -f)”.  Make sure the TAU libraries are before the ADIOS libraries in the ordering.


# Setting up Directories for Dumping Information

// Changes in  workflow.swift

import io;
import launch;
import string;



string envs[] = [ "TAU_PROFILE=1", "PROFILEDIR=/path to the directory/" ];


program1 = "./heat_transfer_adios1";

arguments1 = split("heat  4 3  40 50  6 500", " ");

printf("swift: launching: %s", program1);

exit_code1 = @par=12 launch_envs(program1, arguments1,envs);

printf("swift: received exit code: %d", exit_code1);

if (exit_code1 != 0)

{
  printf("swift: The launched application did not succeed.");
}

else
{
 printf("Entering the second Stage----");
}

string envs1[]= [ "TAU_PROFILE=1", "PROFILEDIR=/path to the directory/" ];

program2 = "stage_write/stage_write";

arguments2 = split("heat.bp staged.bp FLEXPATH \"\" MPI \"\"", " ");

printf("size: %i", size(arguments2));

printf("swift: launching: %s", program2);

exit_code2 = @par=2 launch_envs(program2, arguments2, envs1);

printf("swift: received exit code: %d", exit_code2);

if (exit_code2 != 0)
{
  printf("swift: The launched application did not succeed.");
}
else
{
        printf(" The workflow is done!!!! ");
}



