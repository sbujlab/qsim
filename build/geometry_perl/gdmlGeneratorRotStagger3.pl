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
use vars qw($opt_F $opt_T $data $line @fields @index @x @y @z @dx @dy @dz @rx @ry @rz @quartzCutAngle @refTopOpeningAngle @dzRef @dxLg @dyLg @dzLg  @lgTiltAngle @pmtWindowSize @wallThick @wtOverlap @pmtWindowThickness @range  $i $j $k);

###  Get the option flags.
getopts('F:T:');

if ($#ARGV > -1){
    print STDERR "Unknown arguments specified: @ARGV\nExiting.\n";
    exit;
}


### Reading CSV file containing parameter values
open($data, '<', $opt_F);
$i=0;
while($line= <$data>){
if ($line =~ /^\s*$/) {   # Check for empty lines
    print "String contains 0 or more white-space character and nothing else.\n";
} else {
chomp $line;
@fields = split(",", $line);
$index[$i]=trim($fields[0]);
$x[$i]=trim($fields[1]);
$y[$i]=trim($fields[2]);
$z[$i]=trim($fields[3]);
$dx[$i]=trim($fields[4]);
$dy[$i]=trim($fields[5]);
$dz[$i]=trim($fields[6]);
$rx[$i]=trim($fields[7]);
$ry[$i]=trim($fields[8]);
$rz[$i]=trim($fields[9]);
$quartzCutAngle[$i]=trim($fields[10]);
$refTopOpeningAngle[$i]=trim($fields[11]);
$dzRef[$i]=trim($fields[12]);
$dxLg[$i]=trim($fields[13]);
$dyLg[$i]=trim($fields[14]);
$dzLg[$i]=trim($fields[15]);
$lgTiltAngle[$i]=trim($fields[16]);
$pmtWindowSize[$i]=trim($fields[17]); # Get rid of initial and trailing white spaces
$pmtWindowThickness[$i]= trim($fields[17]);
$i=$i+1;
}
}
####################################################

#### start solids 

# Defining the logical volume. Watch out for this. 
open(def, ">", "solids_${opt_T}.xml") or die "cannot open > solids_${opt_T}.xml: $!";
print def "<solids>\n\n";

print def "<box lunit=\"mm\" name=\"boxMotherSol_${opt_T}\" x=\"20\" y=\"4000\" z=\"4000\"/>\n";
print def "<cone aunit=\"deg\" deltaphi=\"360\" lunit=\"mm\" name=\"coneMotherSol_${opt_T}\" rmax1=\"499\" rmax2=\"499\" rmin1=\"0\" rmin2=\"0\" startphi=\"0\" z=\"20\"/>\n";
print def "<subtraction name =\"logicMotherSol_${opt_T}\">
	<first ref=\"boxMotherSol_${opt_T}\"/> 
	<second ref=\"coneMotherSol_${opt_T}\"/> 
	<position unit=\"mm\" name=\"coneMotherPos_${opt_T}\" x=\"0\" y=\"0\" z=\"0\"\/>
        <rotation unit=\"rad\" name=\"coneMotherRot_${opt_T}\" x=\"0\" y=\"pi/2\" z=\"0\"/>
</subtraction>\n\n";

for $j(0..$i-1){
print def "<box name=\"quartzRecSol_${opt_T}_$j\" x=\"$dx[$j]\" y=\"$dy[$j]\" z=\"$dz[$j]\" lunit= \"mm\" />\n";
if ($quartzCutAngle[$j]>0){
print def "<arb8 name=\"quartzCutSol_${opt_T}_$j\" v1x=\"${dx[$j]*(0.5)}\" v1y=\"${dy[$j]*(0.5)}\" v4x=\"${dx[$j]*(-0.5)}\" v4y=\"${dy[$j]*(0.5)}\" v3x=\"${dx[$j]*(-0.5)} \" v3y=\"${dy[$j]*(-0.5)} \" v2x=\"${dx[$j]*(0.5)}\" v2y=\"${dy[$j]*(-0.5)}\" v5x=\"${dx[$j]*(0.5)}\" v5y=\"${dy[$j]*(0.5)}\" v8x=\"${dx[$j]*(0.5)}\" v8y=\"${dy[$j]*(0.5)}\" v7x=\"${dx[$j]*(0.5)} \" v7y=\"${dy[$j]*(-0.5)}\" v6x=\"${dx[$j]*0.5}\" v6y=\"${dy[$j]*(-0.5)} \" dz=\"${dx[$j]*(0.5*tan($quartzCutAngle[$j]))}\" lunit= \"mm\"/>\n";
}elsif ($quartzCutAngle[$j]<0){
print def "<arb8 name=\"quartzCutSol_${opt_T}_$j\" v1x=\"${dx[$j]*(0.5)}\" v1y=\"${dy[$j]*(0.5)} \" v4x=\"${dx[$j]*(-0.5)}\" v4y=\"${dy[$j]*(0.5)} \" v3x=\"${dx[$j]*(-0.5)}\" v3y=\"${dy[$j]*(-0.5)}\" v2x=\"${dx[$j]*(0.5)}\" v2y=\"${dy[$j]*(-0.5)}\" v5x=\"${dx[$j]*(-0.5)}\" v5y=\"${dy[$j]*0.5}\" v8x=\"${dx[$j]*(-0.5)}\" v8y=\"${dy[$j]*(0.5)}\" v7x=\"${dx[$j]*(-0.5)} \" v7y=\"${dy[$j]*(-0.5)} \" v6x=\"${dx[$j]*(-0.5)}\" v6y=\"${dy[$j]*(-0.5)}\" dz=\"${dx[$j]*0.5*(tan(-$quartzCutAngle[$j]))}\" lunit= \"mm\"/>\n";
}else{
}

print def "<arb8 name=\"refSol_${opt_T}_$j\" v1x=\"",$dx[$j]*(0.5),"\" v1y=\"${dy[$j]*(0.5)} \"  v4x=\"${dx[$j]*(-0.5)}\" v4y=\"${dy[$j]*(0.5)}\" v3x=\"${dx[$j]*(-0.5)}\" v3y=\"${dy[$j]*(-0.5)}\" v2x=\"${dx[$j]*(0.5)}\" v2y=\"${dy[$j]*(-0.5)}\"  v5x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j]),"\" v5y=\"${dyLg[$j]*0.5}\" v8x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j]),"\" v8y=\"${dyLg[$j]*(0.5)}\" v7x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j]),"\" v7y=\"${dyLg[$j]*(-0.5)} \" v6x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j]),"\" v6y=\"${dyLg[$j]*(-0.5)}\" dz=\"",$dzRef[$j]*(0.5),"\" lunit= \"mm\"/>\n";

print def "<arb8 name=\"lgSol1_${opt_T}_$j\" v1x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2,"\" v1y=\"${dyLg[$j]*(0.5)} \"  v4x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/(-2),"\" v4y=\"${dyLg[$j]*(0.5)}\" v3x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/(-2),"\" v3y=\"${dyLg[$j]*(-0.5)}\" v2x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2,"\" v2y=\"${dyLg[$j]*(-0.5)}\" v5x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2-$dxLg[$j]*sin($lgTiltAngle[$j])/tan(pi/2-$lgTiltAngle[$j]),"\" v5y=\"${dyLg[$j]*(0.5)}\" v8x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2-$dxLg[$j]*sin($lgTiltAngle[$j])/tan(pi/2-$lgTiltAngle[$j]),"\" v8y=\"${dyLg[$j]*(0.5)}\" v7x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2-$dxLg[$j]*sin($lgTiltAngle[$j])/tan(pi/2-$lgTiltAngle[$j]),"\" v7y=\"${dyLg[$j]*(-0.5)}\" v6x=\"",$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2-$dxLg[$j]*sin($lgTiltAngle[$j])/tan(pi/2-$lgTiltAngle[$j]),"\" v6y=\"${dyLg[$j]*(-0.5)} \" dz=\"",$dxLg[$j]*sin($lgTiltAngle[$j])/2,"\" lunit= \"mm\"/>\n";


print def "<box name=\"lgSol2_${opt_T}_$j\" x=\"$dxLg[$j]\" y=\"$dyLg[$j]\" z=\"$dzLg[$j]\" lunit= \"mm\" />\n";

=pod
print def "<union name=\"quartzSol_${opt_T}_$j\">
    <first ref=\"quartzRecSol_${opt_T}_$j\"/>
    <second ref=\"quartzCutSol_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"",$dx[$j]*0.5*(tan(abs($quartzCutAngle[$j])))+$dz[$j]*0.5,"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";
=cut

print def "<union name=\"logicSol1_${opt_T}_$j\">
    <first ref=\"quartzRecSol_${opt_T}_$j\"/>
    <second ref=\"refSol_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"",$dzRef[$j]*(0.5)+$dz[$j]*(0.5),"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";


print def "<union name=\"logicSol2_${opt_T}_$j\">
    <first ref=\"logicSol1_${opt_T}_$j\"/>
    <second ref=\"lgSol1_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])/2,"\" y=\"0\" z=\"",$dzRef[$j]+$dz[$j]*(0.5)+$dxLg[$j]*sin($lgTiltAngle[$j])/2,"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";



print def "<union name=\"quartzSol_${opt_T}_$j\">
    <first ref=\"logicSol2_${opt_T}_$j\"/>
    <second ref=\"lgSol2_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/sin(pi/2-$lgTiltAngle[$j])+$dxLg[$j]*cos($lgTiltAngle[$j])/2-$dzLg[$j]*sin($lgTiltAngle[$j])/2,"\" y=\"0\" z=\"",$dzRef[$j]+$dz[$j]*(0.5)+$dzLg[$j]*cos($lgTiltAngle[$j])/2+$dxLg[$j]*sin($lgTiltAngle[$j])/2,"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"",(-1)*$lgTiltAngle[$j],"\" z=\"0\"/> 
</union>\n";

###########




}

print def "\n</solids>";
close(def) or warn "close failed: $!";
#########################################


### Start Structure
open(def, ">", "detector_${opt_T}.gdml") or die "cannot open > detector_${opt_T}.gdml: $!";
print def "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n
<!DOCTYPE gdml [
	<!ENTITY materials SYSTEM \"materialsNew.xml\"> 
	<!ENTITY solids_${opt_T} SYSTEM \"solids_${opt_T}.xml\"> 
]> \n
<gdml xmlns:gdml=\"http://cern.ch/2001/Schemas/GDML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"schema/gdml.xsd\">\n

&materials; 
&solids_${opt_T};\n
<structure>\n";



for $j(0..$i-1){

print def "<volume name=\"quartzVol_${opt_T}\_$j\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"quartzSol_${opt_T}\_$j\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"red\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"$index[$j]\"/>  
</volume>\n";


print def "<volume name=\"quartzCutVol_${opt_T}\_$j\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"quartzCutSol_${opt_T}\_$j\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"red\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"",($index[$j]+1000),"\"/>  
</volume>\n";


}
print def "<volume name=\"logicMotherVol_${opt_T}\"> 
	<materialref ref=\"Air\"/>
	<solidref ref=\"logicMotherSol_${opt_T}\"/>\n";
for $j(0..$i-1){
print def "<physvol name=\"detector_${opt_T}_$j\">
			<volumeref ref=\"quartzVol_${opt_T}_$j\"/>
			<position name=\"detectorPos_${opt_T}_$j\" unit=\"mm\" x=\"$x[$j]\" y=\"$y[$j]\" z=\"$z[$j]\"/>
			<rotation name=\"detectorRot_${opt_T}_$j\" unit=\"rad\" x=\"$rx[$j]\" y=\"$ry[$j]\" z=\"$rz[$j]\"/>
</physvol> \n";
}
print def "</volume>";


print def "\n</structure>\n\n";


# end detector #

print def "<setup name=\"logicMother_${opt_T}\" version=\"1.0\">
	<world ref=\"logicMotherVol_${opt_T}\"/>
</setup>\n
</gdml>";

close(def) or warn "close failed: $!";

sub ltrim { my $s = shift; $s =~ s/^\s+//;       return $s };
sub rtrim { my $s = shift; $s =~ s/\s+$//;       return $s };
sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };




