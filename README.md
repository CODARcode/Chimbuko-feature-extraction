# Chimbuko-feature-extraction
Produce a JSON file for the performance metrics using the TAU profile summary.

# Building TAU with ADIOS

1.	Build ADIOS 1.12 as usual  (source available at https://github.com/ornladios/ADIOS)

2.	 Configure and build TAU 2.26.2 (or highier) as usual. Add the flag -adios=/path/to/adios/installation at the configure step (source available at http://tau.uoregon.edu/tau.tgz) if you want to capture the adios events through TAU.

3.	 Add /path/to/tau/$arch/bin to your PATH/path environment variable  (where “/path/to/tau” is your TAU installation location)

4.	 Set the TAU_MAKEFILE to the Makefile that matches your TAU configuration, located in /path/to/tau/$arch/lib/Makefile.tau-*

# Instrumention using TAU
When linking your executable(s), replace "${ADIOSDIR}/bin/adios_config -l -f” with "tau_cc.sh -tau:showlibs ${ADIOSDIR}/bin/adios_config -l -f” to get both the TAU link flags/libraries as well as the ADIOS flags and libraries.  If replacing in a Makefile, it might look something like this: "$(shell tau_cc.sh -tau:showlibs) $(shell ${ADIOSDIR}/bin/adios_config -l -f)”.  Make sure the TAU libraries are before the ADIOS libraries in the ordering.

Consult https://www.cs.uoregon.edu/research/tau/home.php for more detail. 


# Setting up Directories for Dumping Information
You need to specify using TAU flags, wheither you need to dump profiles or traces. Use "TAU_PROFILE=1" for profiles and "TAU_TRACE=1" for traces. Since in a workflow, there are more than one components using TAU, you need to specify specific directories where you want to dump your profiles/traces from TAU. The TAU infrastructure provides flags for this. Use "PROFILEDIR" to specify the the path to a dirctory for dumping profiles from TAU. Use "TRACEDIR" to specify the path to a directory for  dumping traces from TAU.

Consult https://www.cs.uoregon.edu/research/tau/home.php for complete information on various flags and their usages.

In Savanna, you need to initialize a string specifying your options for information collection. For this purpose a special API, "launch_envs", has been developed. The API takes three arguments as:

**launch_envs(program, arguments, envs);**

where **"program"** is a workflow component, **"arguments"** is the data to be passed to the component at runtime, and **"envs"** is the string which specify flags for the program.

## Example
// Changes in  the Heat Tranfer workflow.swift

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

# Performance Metrics
## Overall information
- Total execution time
- Name 
- Version
- Code Regions
- Call trees
- Components
- Input and output files including version, size, location etc.
## System Architecture Description
- Name
- Number of nodes running on 
- Aggregate time spent computing
- Aggregate time spent communicating
- Aggregate time spent idle
- Execution time per application per node
- Idle times, per application, per node, per code region
- Memory usage
- Programming paradigm
- Message performance - number, message size, wait time, etc.
- Interconnect overall performance, over all load, application specific load and performance
## For each pair of components
- Aggregate communication volume
- Total number of messages

# Python script to extract information
## feature_extraction2json.py

The script extracts the performance metrics.

-feature_extraction2json -o merged.json  [d1] [d2]

-where [d1] & [d2] are the directories containing the TAU profiles.
