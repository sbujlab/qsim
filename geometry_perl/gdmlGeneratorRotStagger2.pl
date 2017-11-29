: # feed this into perl *-*-perl-*-*
    eval 'exec perl -S $0 "$@"'
    if $running_under_some_shell;

use Cwd;
use Cwd 'abs_path';
use File::Find ();
use File::Basename;
use Math::Trig;
use Getopt::Std;

### Declare variables explicitly so "my" not needed
use strict 'vars';
use vars qw($opt_D @ringname @numDetPerRing @quartzRad @quartzZ @quartzThickness @quartzHeight @quartzWidth @quartzCutAngle @quartzTiltAngle @lgTiltAngle @refOpeningAngle @refLength @lgLength @zRing @detWidth @pmtWindowSize @wallThick @wtOverlap @pmtWindowThickness @range $x $y $z $i $j $k);

###  Get the option flags.
getopts('D:');

if ($#ARGV > -1){
    print STDERR "Unknown arguments specified: @ARGV\nExiting.\n";
    exit;
}

@range = (0,18); #range of subrings to draw
@ringname = ("1a", "1b", "2a", "2b", "3a", "3b", "4a", "4b", "5oa", "5ob", "5oc", "5ta", "5tb", "5tc", "5ca", "5cb", "5cc",  "6a", "6b"); 
@numDetPerRing = (14, 14, 14, 14, 14, 14, 14, 14, 7, 7, 7, 14, 14, 14,  7,7,7, 14,14); # number of detectors per subring. If this is changed line 193 to line 203 may need to be changed as well. 
@quartzTiltAngle = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); # angle of tilt of quartz with respect to xy-plane
@quartzThickness =  (10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10); # thickness of quartz piece along z-axis
@quartzHeight =  (40,40, 50, 50, 55, 55, 75, 75, 105, 105,105, 115, 115, 115, 140, 140, 140, 100, 100); # height of quartz piece
@wtOverlap = (30.5, 30.5, 30.5, 30.5, 30, 30, 29.5, 29.5, 117.6, 117.6, 117.6, 117.6, 117.6, 117.6, 117.6, 117.6, 117.6, 29, 29); # weight factor to avoid azimuthal overlap. must be equal to or greater than the number of detectors in the ring
@quartzRad = (710, 710, 755, 755, 817.5, 817.5, 892.5, 892.5, 987.5, 987.5, 987.5,  1017.5, 1017.5, 1017.5, 1030, 1030, 1030, 1150, 1150); # radial position of quartz ring
for $i($range[0]..$range[1]){
$quartzWidth[$i]=  pi*(($quartzRad[$i]+$quartzHeight[$i]/2)*($quartzRad[$i]+$quartzHeight[$i]/2)-($quartzRad[$i]-$quartzHeight[$i]/2)*($quartzRad[$i]-$quartzHeight[$i]/2))/($wtOverlap[$i]*$quartzHeight[$i]); # calculate azimuthal width of quartz piece
}  
@quartzZ = (655, 680 , 515 , 540, 375, 399.724 , 265, 290, 165, 115, 170, 190, 140, 200, 165, 115, 170,  25, 0); # position of the quartz ring along z-axis. Positive implies downstream. 
@quartzCutAngle = (60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60); # light guide tilt angle with respect to quartz piece
@lgTiltAngle = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); # light guide tilt angle with respect to quartz piece. 
@lgLength = (200, 200, 385, 385, 325, 325, 285, 285, 165, 165, 165, 165, 165, 165, 165, 165, 165,  265, 265); # length of light guide
@refOpeningAngle = (19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19); # opening angle of reflector
@refLength = (35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 100); # length of reflector
@pmtWindowSize= (77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2,77.2); # length of one side of square pmt window
@pmtWindowThickness= (3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3); # thickness of pmt window which is an aluminium box with circular quartz window and detector at the center placed at the end of the lightguide.
@wallThick = (1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1); #thickness of wall of reflector and light guide


# Start definitions #

open(def, ">", "definitionsNew.xml") or die "cannot open > definitionsNew.xml: $!";
print def "<define>\n\n";

print  def "<matrix name=\"pmtSize\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$pmtWindowSize[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

 
print  def "<matrix name=\"pmtThickness\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$pmtWindowThickness[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}


print  def "<matrix name=\"quartzTiltAngle\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$quartzTiltAngle[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

print  def "<matrix name=\"quartzCutAngle\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$quartzCutAngle[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}


print  def "<matrix name=\"lgTiltAngle\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$lgTiltAngle[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

print  def "<matrix name=\"lgLength\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$lgLength[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

print  def "<matrix name=\"refOpeningAngle\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$refOpeningAngle[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

print  def "<matrix name=\"refLength\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$refLength[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}

print  def "<matrix name=\"refLgWallThick\" coldim=\"1\" values=\"";
for $j(0..18){
if($j!=0) {print def "                                              ";} 
print def "$wallThick[$j]";
if($j==18){ print def "\"/>\n\n";  }
else { print def "\n";}
}


print def "<position name=\"center\"/>\n";
print def "<rotation name=\"identity\"/>\n\n";


print def "<matrix coldim=\"2\" name=\"absLengthQuartz\" values=\"1.54986e-06 263160
						1.77127e-06 250000 
						2.06648e-06 227270 
						2.47978e-06 200000 
						3.09973e-06 131580 
						4.13297e-6 16130 
						4.95956e-6 740 
						5.51063e-6 125
						5.90424e-6 10\"/>\n";
print def "<matrix coldim=\"2\" name=\"rIndexQuartz\" values=\"1.54986e-06 1.45338
						1.77127e-06 1.45536 
						2.06648e-06 1.4581 
						2.47978e-06 1.46239
						3.09973e-06 1.47018 
						4.13297e-6  1.48786
						4.95956e-6  1.50751
						5.51063e-6 1.52422 
						5.90424e-6 1.53842\"/> \n";
print def "<matrix coldim=\"2\" name=\"absLengthAir\" values=\"1.54986e-06 1e+06 
						1.77127e-06 1e+06
						2.06648e-06 1e+06
						2.47978e-06 1e+06 
						3.09973e-06 1e+06 
						4.13297e-6 1e+06 
						4.95956e-6 1e+06 
						5.51063e-6 1e+06 
						5.90424e-6 1e+06\"/> \n";
print def "<matrix coldim=\"2\" name=\"fastComponentAir\" values=\"1.54986e-06 0.1 
						1.77127e-06 0.1 
						2.06648e-06 0.1 
						2.47978e-06 0.1 
						3.09973e-06 0.1 
						4.13297e-06 0.1
						4.95956e-06 0.1 
						5.51063e-6 0.1 
						5.90424e-6 0.1\"/> \n";
print def "<matrix coldim=\"2\" name=\"rIndexAir\" values=\"1.54986e-06 1.000292 
						1.77127e-06 1
                                                2.06648e-06 1.000292 
						2.47978e-06 1.000292 
						3.09973e-06 1.000292
						4.13297e-06 1.000292
						4.95956e-06 1.000292 
						5.51063e-6 1.000292 
						5.90424e-6 1.000292\"/> \n";
print def "<matrix coldim=\"2\" name=\"slowComponentAir\" values=\"1.54986e-06 0.1 
						1.77127e-06 0.1 
						2.06648e-06 0.1
						2.47978e-06 0.1 
						3.09973e-06 0.1 
						4.13297e-06 0.1 
						4.95956e-06 0.1
						5.51063e-6 0.1 
						5.90424e-6 0.1\"/> \n";



## Forming the matrix defining quartz dimensions ##
for $j($range[0]..$range[1]){ 
print  def "<matrix name=\"quartzDim_$j\" coldim=\"3\" values=\"";
for $i(1..$numDetPerRing[$j]){
if($i!=1) {print def "                                              ";} 
print def "$quartzHeight[$j] $quartzWidth[$j] $quartzThickness[$j]";
if($i==$numDetPerRing[$j]){ print def "\"/>\n\n";  }
else { print def "\n";}
}
}
##------------------------------------------------##

## Forming the matrix defining quartz positions ##
for $j($range[0]..$range[1]){ 
print  def "<matrix name=\"quartzPos_$j\" coldim=\"3\" values=\"";
for $i(1..$numDetPerRing[$j]){
$x = $quartzRad[$j];
if ($j==0||$j==2||$j==4||$j==6||$j==17){
$y = 2*($i-1)*2*pi/($numDetPerRing[$j]+$numDetPerRing[$j+1]);
} elsif ($j==1||$j==3||$j==5||$j==7||$j==18){
$y = (2*($i-1)+1)*2*pi/($numDetPerRing[$j]+$numDetPerRing[$j-1]);
} elsif ($j==8||$j==9||$j==10|| $j==14 || $j==15 || $j==16) {
$y= (12*($i-1)+($j-8)-1)*2*pi/($numDetPerRing[8]+$numDetPerRing[9]+$numDetPerRing[10]+ $numDetPerRing[11]+ $numDetPerRing[12]+$numDetPerRing[13]+$numDetPerRing[14]+$numDetPerRing[15]+ $numDetPerRing[16]);
} else {
$y= (6*($i-1)+($j-8)-1)*2*pi/($numDetPerRing[8]+$numDetPerRing[9]+$numDetPerRing[10]+ $numDetPerRing[11]+ $numDetPerRing[12]+$numDetPerRing[13]+$numDetPerRing[14]+$numDetPerRing[15]+ $numDetPerRing[16]); 
}

$z = $quartzZ[$j];
if($i!=1) {print def "                        ";} 
print def "$x $y $z";
if($i==$numDetPerRing[$j]){ print def "\"/>\n\n";  }
else { print def "\n";}
}
} 
##----------------------------------------------##

## Forming the matrix defining quartz rotations ##
for $j($range[0]..$range[1]){ 
print  def "<matrix name=\"quartzRot_$j\" coldim=\"3\" values=\"";
for $i(1..$numDetPerRing[$j]){
if($i!=1) {print def "                       ";} 
print def "quartzPos_$j\[$i,2\] quartzTiltAngle\[$j+1\]*pi/180  0";
if($i==$numDetPerRing[$j]){ print def "\"/>\n\n";  }
else { print def "\n";}
}
}  
##--------------------------------------------------## 
print def "</define>";
close(def) or warn "close failed: $!";

# end definitions #



# start materials #


open(def, ">", "materialsNew.xml") or die "cannot open > materialsNew.xml: $!";
print def "<materials>\n\n";

print def "<material Z=\"82\" name=\"Lead\" state=\"solid\">
      <MEE unit=\"eV\" value=\"823\"/>
      <D unit=\"g/cm3\" value=\"11.35\"/>
      <atom unit=\"g/mole\" value=\"207.19\"/>
</material>
    
<element Z=\"1\" name=\"Hydrogen\">
    <atom unit=\"g/mole\" value=\"1.01\"/>
</element>

<element Z=\"6\" name=\"Carbon\">
    <atom unit=\"g/mole\" value=\"12.011\"/>
</element>

<material name=\"Tyvek\" state=\"solid\">
    <MEE unit=\"eV\" value=\"56.5182975271737\"/> 
    <D unit=\"g/cm3\" value=\"0.96\"/>
    <fraction n=\"0.14396693036847\" ref=\"Hydrogen\"/>
    <fraction n=\"0.85603306963153\" ref=\"Carbon\"/>
</material>

<element Z=\"14\" name=\"Silicon\">
    <atom unit=\"g/mole\" value=\"28.09\"/>
</element>

<element Z=\"8\" name=\"Oxygen\">
    <atom unit=\"g/mole\" value=\"16\"/>
</element>

<material name=\"Quartz\" state=\"solid\">
    <MEE unit=\"eV\" value=\"125.663004061076\"/>
    <D unit=\"g/cm3\" value=\"2.2\"/>
    <fraction n=\"0.467465468463971\" ref=\"Silicon\"/>
    <fraction n=\"0.532534531536029\" ref=\"Oxygen\"/>
</material>

<element Z=\"7\" name=\"Nitrogen\">
    <atom unit=\"g/mole\" value=\"14.01\"/>
</element>

<material name=\"Air\" state=\"gas\">
    <MEE unit=\"eV\" value=\"85.7030667332999\"/>
    <D unit=\"g/cm3\" value=\"0.00129\"/>
    <fraction n=\"0.7\" ref=\"Nitrogen\"/>
    <fraction n=\"0.3\" ref=\"Oxygen\"/>
</material>

<material Z=\"13\" name=\"Aluminium\" state=\"solid\">
    <MEE unit=\"eV\" value=\"166\"/>
    <D unit=\"g/cm3\" value=\"2.7\"/>
    <atom unit=\"g/mole\" value=\"26.98\"/>
</material>

<material Z=\"19\" name=\"Photocathode\" state=\"solid\">
      <MEE unit=\"eV\" value=\"190\"/>
      <D unit=\"g/cm3\" value=\"5\"/>
      <atom unit=\"g/mole\" value=\"39.0983\"/>
</material>";
 

print def "\n</materials>";
close(def) or warn "close failed: $!";


# end materials #



# start solids #
open(def, ">", "solidsNew.xml") or die "cannot open > solidsNew.xml: $!";
print def "<solids>\n\n";


print def "<box lunit=\"mm\" name=\"boxDetectors\" x=\"1500\" y=\"4000\" z=\"4000\"/>\n";
print def "<cone aunit=\"deg\" deltaphi=\"360\" lunit=\"mm\" name=\"coneMother_sub_1\" rmax1=\"499\" rmax2=\"499\" rmin1=\"0\" rmin2=\"0\" startphi=\"0\" z=\"1500\"/>\n";
print def "<subtraction name =\"logicDetSol\">
	<first ref=\"boxDetectors\"/> 
	<second ref=\"coneMother_sub_1\"/> 
	<position unit=\"mm\" name=\"conMother_sub_1_Pos\" x=\"0\" y=\"0\" z=\"0\"\/>
        <rotation unit=\"rad\" name=\"conMother_sub_1_Rot\" x=\"0\" y=\"pi/2\" z=\"0\"/>
    </subtraction>\n\n";

#print def "<box name=\"logicDetSol\" x=\"1500\" y=\"4000\" z=\"4000\" lunit=\"mm\"/>\n\n";





for $j($range[0]..$range[1]){
for $i(1..$numDetPerRing[$j]){
print def "<box name=\"quartzRec_$j\_$i\" x=\"quartzDim_$j\[$i,3\]\" y=\"quartzDim_$j\[$i,2\]\" z=\"quartzDim_$j\[$i,1\]\" lunit= \"mm\" />\n";


if ($quartzCutAngle[$j]>0){
print def "<arb8 name=\"quartzCut_$j\_$i\" v1x=\"quartzDim_$j\[$i,3\]/2\" v1y=\"quartzDim_$j\[$i,2\]/2 \" v4x=\"-quartzDim_$j\[$i,3\]/2\" v4y=\"quartzDim_$j\[$i,2\]/2 \" v3x=\"-quartzDim_$j\[$i,3\]/2 \" v3y=\"-quartzDim_$j\[$i,2\]/2 \" v2x=\"quartzDim_$j\[$i,3\]/2\" v2y=\"-quartzDim_$j\[$i,2\]/2\" v5x=\"quartzDim_$j\[$i,3\]/2\" v5y=\"quartzDim_$j\[$i,2\]/2\" v8x=\"quartzDim_$j\[$i,3\]/2 \" v8y=\"quartzDim_$j\[$i,2\]/2\" v7x=\"quartzDim_$j\[$i,3\]/2 \" v7y=\"-quartzDim_$j\[$i,2\]/2 \" v6x=\"quartzDim_$j\[$i,3\]/2 \" v6y=\"-quartzDim_$j\[$i,2\]/2 \" dz=\"quartzDim_$j\[$i,3\]*tan(quartzCutAngle[$j+1]*pi/180)/2\" lunit= \"mm\"/>\n";
}elsif ($quartzCutAngle[$j]<0){
print def "<arb8 name=\"quartzCut_$j\_$i\" v1x=\"quartzDim_$j\[$i,3\]/2\" v1y=\"quartzDim_$j\[$i,2\]/2 \" v4x=\"-quartzDim_$j\[$i,3\]/2\" v4y=\"quartzDim_$j\[$i,2\]/2 \" v3x=\"-quartzDim_$j\[$i,3\]/2 \" v3y=\"-quartzDim_$j\[$i,2\]/2 \" v2x=\"quartzDim_$j\[$i,3\]/2\" v2y=\"-quartzDim_$j\[$i,2\]/2\" v5x=\"-quartzDim_$j\[$i,3\]/2\" v5y=\"quartzDim_$j\[$i,2\]/2\" v8x=\"-quartzDim_$j\[$i,3\]/2 \" v8y=\"quartzDim_$j\[$i,2\]/2\" v7x=\"-quartzDim_$j\[$i,3\]/2 \" v7y=\"-quartzDim_$j\[$i,2\]/2 \" v6x=\"-quartzDim_$j\[$i,3\]/2 \" v6y=\"-quartzDim_$j\[$i,2\]/2 \" dz=\"quartzDim_$j\[$i,3\]*tan(-quartzCutAngle[$j+1]*pi/180)/2\" lunit= \"mm\"/>\n";
}else{
}


print def "<arb8 name=\"refOuterSol_$j\_$i\" v1x=\"quartzDim_$j\[$i,3\]/2\" v1y=\"quartzDim_$j\[$i,2\]/2 \" v4x=\"-quartzDim_$j\[$i,3\]/2\" v4y=\"quartzDim_$j\[$i,2\]/2 \" v3x=\"-quartzDim_$j\[$i,3\]/2 \" v3y=\"-quartzDim_$j\[$i,2\]/2 \" v2x=\"quartzDim_$j\[$i,3\]/2\" v2y=\"-quartzDim_$j\[$i,2\]/2\" v5x=\"4*quartzDim_$j\[$i,3\]/2\" v5y=\"quartzDim_$j\[$i,2\]/2\" v8x=\"-2*quartzDim_$j\[$i,3\]/2 \" v8y=\"quartzDim_$j\[$i,2\]/2 \" v7x=\"-2*quartzDim_$j\[$i,3\]/2 \" v7y=\"-quartzDim_$j\[$i,2\]/2 \" v6x=\"4*quartzDim_$j\[$i,3\]/2 \" v6y=\"-quartzDim_$j\[$i,2\]/2 \" dz=\"refLength[$j+1]/2\" lunit= \"mm\"/>\n";
print def "<arb8 name=\"refInnerSol_$j\_$i\" v1x=\"(quartzDim_$j\[$i,3\]-refLgWallThick[$j+1])/2\" v1y=\"(quartzDim_$j\[$i,2\]-refLgWallThick[$j+1])/2 \" v4x=\"(-quartzDim_$j\[$i,3\]+refLgWallThick[$j+1])/2\" v4y=\"(quartzDim_$j\[$i,2\]/2-refLgWallThick[$j+1]) \" v3x=\"(-quartzDim_$j\[$i,3\]+refLgWallThick[$j+1])/2 \" v3y=\"(-quartzDim_$j\[$i,2\]+refLgWallThick[$j+1])/2 \" v2x=\"(quartzDim_$j\[$i,3\]-refLgWallThick[$j+1])/2\" v2y=\"(-quartzDim_$j\[$i,2\]+refLgWallThick[$j+1])/2\" v5x=\"(quartzDim_$j\[$i,3\]-refLgWallThick[$j+1])/2\" v5y=\"(quartzDim_$j\[$i,2\]-refLgWallThick[$j+1])/2\" v8x=\"(-quartzDim_$j\[$i,3\]+refLgWallThick[$j+1])/2 \" v8y=\"(quartzDim_$j\[$i,2\]-refLgWallThick[$j+1])/2 \" v7x=\"(-quartzDim_$j\[$i,3\]+refLgWallThick[$j+1])/2 \" v7y=\"(-quartzDim_$j\[$i,2\]+refLgWallThick[$j+1])/2 \" v6x=\"(quartzDim_$j\[$i,3\]-refLgWallThick[$j+1])/2 \" v6y=\"(-quartzDim_$j\[$i,2\]+refLgWallThick[$j+1])/2 \" dz=\"refLength[$j+1]/2\" lunit= \"mm\"/>\n";


=pod
print def "<trd name=\"refOuterSol_$j\_$i\" x1=\"quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))\" y1=\"quartzDim_$j\[$i,2\]\" x2=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))\" y2=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,2\]\" z=\"refLength[$j+1]\" lunit=\"mm\" />\n";

print def "<trd name=\"refInnerSol_$j\_$i\" x1=\"quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))-refLgWallThick[$j+1]\" y1=\"quartzDim_$j\[$i,2\]-refLgWallThick[$j+1]\" x2=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))-refLgWallThick[$j+1]\" y2=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,2\]-refLgWallThick[$j+1]\" z=\"refLength[$j+1]\" lunit=\"mm\" />\n";
=cut

print def "<subtraction name=\"refSol_$j\_$i\">
         <first ref=\"refOuterSol_$j\_$i\"/>
         <second ref=\"refInnerSol_$j\_$i\"/>
         <positionref ref=\"center\"/>
         <rotationref ref=\"identity\"/>
</subtraction>\n"; 

print def "<trd name=\"lgOuterSol_$j\_$i\" x1=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))\" y1=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,2\]\" x2=\"pmtSize[$j+1]\" y2=\"pmtSize[$j+1]\" z=\"lgLength[$j+1]\"/>\n";

print def "<trd name=\"lgInnerSol_$j\_$i\" x1=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,3\]/(cos(lgTiltAngle[$j+1]*pi/180))-refLgWallThick[$j+1]\" y1=\"2*refLength[$j+1]*tan(refOpeningAngle[$j+1]*pi/180)+quartzDim_$j\[$i,2\]-refLgWallThick[$j+1]\" x2=\"pmtSize[$j+1]-refLgWallThick[$j+1]\" y2=\"pmtSize[$j+1]-refLgWallThick[$j+1]\" z=\"lgLength[$j+1]\"/>\n";

print def "<subtraction name=\"lgSol_$j\_$i\">
         <first ref=\"lgOuterSol_$j\_$i\"/>
         <second ref=\"lgInnerSol_$j\_$i\"/>
         <positionref ref=\"center\"/>
         <rotationref ref=\"identity\"/>
</subtraction>\n"; 


print def "<box name=\"endCapSol_$j\_$i\" x=\"pmtSize[$j+1]\" y=\"pmtSize[$j+1]\" z=\"pmtThickness[$j+1]\" lunit=\"mm\"/>\n";
print def "<tube name=\"pmtContainerSol_$j\_$i\" aunit=\"deg\" deltaphi=\"360\" lunit=\"mm\" rmax=\"(pmtSize[$j+1]-refLgWallThick[$j+1])/2\" rmin=\"0\" startphi=\"0\" z=\"pmtThickness[$j+1]\"/>\n";
print def "<subtraction name=\"endCapFrameSol_$j\_$i\">
         <first ref=\"endCapSol_$j\_$i\"/>
         <second ref=\"pmtContainerSol_$j\_$i\"/>
         <positionref ref=\"center\"/>
         <rotationref ref=\"identity\"/>
</subtraction>\n"; 
print def "<tube name=\"pmtDetectorSol_$j\_$i\" aunit=\"deg\" deltaphi=\"360\" lunit=\"mm\" rmax=\"(pmtSize[$j+1]-refLgWallThick[$j+1])/2\" rmin=\"0\" startphi=\"0\" z=\"pmtThickness[$j+1]/3\"/>\n";
print def "<tube name=\"pmtWindowSol_$j\_$i\" aunit=\"deg\" deltaphi=\"360\" lunit=\"mm\" rmax=\"(pmtSize[$j+1]-refLgWallThick[$j+1])/2\" rmin=\"0\" startphi=\"0\" z=\"2*pmtThickness[$j+1]/3\"/>\n";




## Forming the logical container for a detector system ##
if ($quartzCutAngle[$j]!=0){
print def "<union name=\"quartzBol_$j\_$i\">
    <first ref=\"quartzRec_$j\_$i\"/>
    <second ref=\"quartzCut_$j\_$i\"/>
    <position name=\"quartzCutPos_$j\_$i\" unit=\"mm\" x=\"0\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(quartzCutAngle[$j+1]*pi/180))/2\"/>
    <rotation name=\"quartzCutRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n"; 
} else {
}
print def "<union name=\"quartzDol_$j\_$i\">
    <first ref=\"quartzRec_$j\_$i\"/>
    <second ref=\"refOuterSol_$j\_$i\"/>
    <position name=\"quartzCutDolPos_$j\_$i\" unit=\"mm\" x=\"0\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+refLength[$j+1]/2\"/>
    <rotation name=\"quartzCutDolRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";




print def "<union name=\"quartzMol_$j\_$i\">
    <first ref=\"quartzDol_$j\_$i\"/>
    <second ref=\"lgOuterSol_$j\_$i\"/>
    <position name=\"quartzCutMolPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+1*lgLength[$j+1])*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+1*lgLength[$j+1])*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
    <rotation name=\"quartzCutMolRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"-lgTiltAngle[$j+1]*pi/180\" z=\"0\"/> 
</union>\n"; 

print def "<union name=\"quartzSol_$j\_$i\">
    <first ref=\"quartzMol_$j\_$i\"/>
    <second ref=\"endCapSol_$j\_$i\"/>
    <position name=\"quartzCutSolPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+2*lgLength[$j+1]+pmtThickness[$j+1])*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+2*lgLength[$j+1]+pmtThickness[$j+1])*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
    <rotation name=\"quartzCutSolRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"-lgTiltAngle[$j+1]*pi/180\" z=\"0\"/> 
</union>\n"; 





## -------------------------------------------------- ##



} 
}
print def "\n</solids>";
close(def) or warn "close failed: $!";

# end solids #


# start detector #

open(def, ">", "detectorNew.gdml") or die "cannot open > detectorNew.gdml: $!";
print def "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n
<!DOCTYPE gdml [
	<!ENTITY definitions SYSTEM \"definitionsNew.xml\">
	<!ENTITY materials SYSTEM \"materialsNew.xml\"> 
	<!ENTITY solids SYSTEM \"solidsNew.xml\"> 
]> \n
<gdml xmlns:gdml=\"http://cern.ch/2001/Schemas/GDML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"schema/gdml.xsd\">\n
&definitions;
&materials; 
&solids;\n
<structure>\n";

for $j($range[0]..$range[1]){
for $i(1..$numDetPerRing[$j]){
$k= $j*50+$i;
$k =4000+$k;
if ($quartzCutAngle[$j]!=0){
print def "<volume name=\"detVol_$j\_$i\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"quartzBol_$j\_$i\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"red\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>  
</volume>\n";
} else {
print def "<volume name=\"detVol_$j\_$i\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"quartzRec_$j\_$i\"/>
         <auxiliary auxtype=\"Color\" auxvalue=\"red\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>  
</volume>\n";
}

$k = 1000+$k;

print def "<volume name=\"refVol_$j\_$i\">
           <materialref ref=\"Aluminium\"/>
           <solidref ref=\"refSol_$j\_$i\"/>
           <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
           <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/> 
</volume>\n";

$k = 1000+$k;
print def "<volume name=\"refInnerVol_$j\_$i\">
           <materialref ref=\"Air\"/>
           <solidref ref=\"refInnerSol_$j\_$i\"/>
           <auxiliary auxtype=\"Color\" auxvalue=\"magenta\"/>  
           <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
           <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>  
</volume>\n";

$k = 1000+$k;

print def "<volume name=\"lgVol_$j\_$i\">
           <materialref ref=\"Aluminium\"/>
           <solidref ref=\"lgSol_$j\_$i\"/>  
           <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
           <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>          
</volume>\n";

$k = 1000+$k;
print def "<volume name=\"lgInnerVol_$j\_$i\">
           <materialref ref=\"Air\"/>
           <solidref ref=\"lgInnerSol_$j\_$i\"/>  
           <auxiliary auxtype=\"Color\" auxvalue=\"magenta\"/>  
           <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
           <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>         
</volume>\n";



print def "<volume name=\"endCapVol_$j\_$i\">
		<materialref ref=\"Aluminium\"/>
		<solidref ref=\"endCapFrameSol_$j\_$i\"/>
</volume>\n";

print def "<volume name=\"pmtWindowVol_$j\_$i\">
		<materialref ref=\"Quartz\"/>
		<solidref ref=\"pmtWindowSol_$j\_$i\"/>
                <auxiliary auxtype=\"Color\" auxvalue=\"red\"/>  
</volume>\n";

$k=1000+$k;
print def "<volume name=\"pmtDetectorVol_$j\_$i\">
		<materialref ref=\"Photocathode\"/>
		<solidref ref=\"pmtDetectorSol_$j\_$i\"/>
                <auxiliary auxtype=\"Color\" auxvalue=\"blue\"/> 
                <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
                <auxiliary auxtype=\"DetNo\" auxvalue=\"$k\"/>    
</volume>\n";




print def "<volume name=\"quartzVol_$j\_$i\"> 
	<materialref ref=\"Air\"/>
	<solidref ref=\"quartzDol_$j\_$i\"/> 
        <physvol name=\"det_$j\_$i\">
			<volumeref ref=\"detVol_$j\_$i\"/>
			<position name=\"detPos_$j\_$i\" unit=\"mm\" x=\"0\" y=\"0\" z=\"0\"/>
			<rotation name=\"detRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/>
         </physvol>
</volume>\n";
=pod
            <physvol name=\"ref_$j\_$i\">
			<volumeref ref=\"refVol_$j\_$i\"/>
			<position name=\"refPos_$j\_$i\" unit=\"mm\" x=\"refLength[$j+1]*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+refLength[$j+1]*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"refRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
         <physvol name=\"refInner_$j\_$i\">
			<volumeref ref=\"refInnerVol_$j\_$i\"/>
			<position name=\"refInnerPos_$j\_$i\" unit=\"mm\" x=\"refLength[$j+1]*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+refLength[$j+1]*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"refInnerRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
         
          <physvol name=\"lg_$j\_$i\">
			<volumeref ref=\"lgVol_$j\_$i\"/>
			<position name=\"lgPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+1*lgLength[$j+1])*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+1*lgLength[$j+1])*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"lgRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
             <physvol name=\"lgInner_$j\_$i\">
			<volumeref ref=\"lgInnerVol_$j\_$i\"/>
			<position name=\"lgInnerPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+1*lgLength[$j+1])*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+1*lgLength[$j+1])*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"lgInnerRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
      
         <physvol name=\"endCap_$j\_$i\">
			<volumeref ref=\"endCapVol_$j\_$i\"/>
			<position name=\"endCapPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+2*lgLength[$j+1]+pmtThickness[$j+1])*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+2*lgLength[$j+1]+pmtThickness[$j+1])*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"endCapRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
          <physvol name=\"pmtWindow_$j\_$i\">
			<volumeref ref=\"pmtWindowVol_$j\_$i\"/>
			<position name=\"pmtWindowPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+2*lgLength[$j+1]++2*pmtThickness[$j+1]/3)*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+2*lgLength[$j+1]+2*pmtThickness[$j+1]/3)*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"pmtWindowRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
         <physvol name=\"pmtDetector_$j\_$i\">
			<volumeref ref=\"pmtDetectorVol_$j\_$i\"/>
			<position name=\"pmtDetectorPos_$j\_$i\" unit=\"mm\" x=\"(2*refLength[$j+1]+2*lgLength[$j+1]++5*pmtThickness[$j+1]/3)*sin(-lgTiltAngle[$j+1]*pi/180)/2\" y=\"0\" z=\"quartzDim_$j\[$i,1\]/2+quartzDim_$j\[$i,3\]*tan(abs(lgTiltAngle[$j+1]*pi/180))/2+(2*refLength[$j+1]+2*lgLength[$j+1]+5*pmtThickness[$j+1]/3)*cos(lgTiltAngle[$j+1]*pi/180)/2\"/>
			<rotation name=\"pmtDetectorRot_$j\_$i\" unit=\"rad\" x=\"0\" y=\"lgTiltAngle[$j+1]*pi/180\" z=\"0\"/>
         </physvol> 
</volume>\n";
=cut


} 
}



print def "<volume name=\"logicDetVol\"> 
	<materialref ref=\"Air\"/>
	<solidref ref=\"logicDetSol\"/>\n";
for $j($range[0]..$range[1]){
for $i(1..$numDetPerRing[$j]){
print def "<physvol name=\"quartzDet_$j\_$i\">
			<volumeref ref=\"quartzVol_$j\_$i\"/>
			<position name=\"quartzPos_$j\_$i\" unit=\"mm\" x=\"quartzPos_$j\[$i,3\]\" y=\"quartzPos_$j\[$i,1\]*sin(quartzPos_$j\[$i,2\])\" z=\"quartzPos_$j\[$i,1\]*cos(quartzPos_$j\[$i,2\])\"/>
			<rotation name=\"quartzRot_$j\_$i\" unit=\"rad\" x=\"quartzRot_$j\[$i,1\]\" y=\"quartzRot_$j\[$i,2\]\" z=\"quartzRot_$j\[$i,3\]\"/>
</physvol> \n";
} 
}
print def "</volume>";



print def "\n</structure>\n";


# end detector #

print def "<setup name=\"logicDet\" version=\"1.0\">
	<world ref=\"logicDetVol\"/>
</setup>\n
</gdml>";

close(def) or warn "close failed: $!";




