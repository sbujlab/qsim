# analysis code
* analysis.sh	    -- a shell script that call other executable files to do all the jobs in one step
* convert_layer.cc  -- extract layer info from the simulation output, the output is **layer.root**
* layer.cc	    -- draw plots using data from **layer.root**
* plot.cc	    -- extract upstream monitor info and draw plots
* z.cc		    -- analysis the z position of particles detected at the upstream monitor detector


# compile all files
> make


# add new code
just add you new filename (root code) in the **makefile** as:
    OBJECT += "new file"
