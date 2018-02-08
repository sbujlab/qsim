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
use vars qw($opt_F $opt_T $data $line @fields @index @x @y @z @dx @dy @dz @rx @ry @rz @quartzCutAngle @refTopOpeningAngle @dzRef @dxLg @dyLg @dzLg  @lgTiltAngle @pmtWindowSize @wallThick @wtOverlap @pmtWindowThickness @range  $i $j $k $width);

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

$width=25;
####################################################


#### start solids 

# Defining the logical volume. Watch out for this. 
open(def, ">", "solids_${opt_T}.xml") or die "cannot open > solids_${opt_T}.xml: $!";
print def "<solids>\n\n";

print def "<box lunit=\"mm\" name=\"boxMotherSol_${opt_T}\" x=\"1000\" y=\"4000\" z=\"4000\"/>\n";
print def "<cone aunit=\"rad\" deltaphi=\"2*pi\" lunit=\"mm\" name=\"coneMotherSol_${opt_T}\" rmax1=\"499\" rmax2=\"499\" rmin1=\"0\" rmin2=\"0\" startphi=\"0\" z=\"20\"/>\n";
print def "<subtraction name =\"logicMotherSol_${opt_T}\">
	<first ref=\"boxMotherSol_${opt_T}\"/> 
	<second ref=\"coneMotherSol_${opt_T}\"/> 
	<position unit=\"mm\" name=\"coneMotherPos_${opt_T}\" x=\"0\" y=\"0\" z=\"0\"\/>
        <rotation unit=\"rad\" name=\"coneMotherRot_${opt_T}\" x=\"0\" y=\"pi/2\" z=\"0\"/>
</subtraction>\n\n";

for $j(0..$i-1){
print def "<box name=\"quartzRecSol_${opt_T}_$j\" x=\"$dx[$j]\" y=\"$dy[$j]\" z=\"$dz[$j]\" lunit= \"mm\" />\n";

print def "<arb8 name=\"quartzCutSol_${opt_T}_$j\" v1x=\"",$dx[$j]*(0.5),"\" v1y=\"",$dy[$j]*(0.5),"\" v4x=\"",$dx[$j]*(-0.5),"\" v4y=\"",$dy[$j]*(0.5),"\" v3x=\"",$dx[$j]*(-0.5),"\" v3y=\"",$dy[$j]*(-0.5),"\" v2x=\"",$dx[$j]*(0.5),"\" v2y=\"",$dy[$j]*(-0.5),"\" v5x=\"",$dx[$j]*(0.5),"\" v5y=\"",$dy[$j]*(0.5),"\" v8x=\"",$dx[$j]*(0.5),"\" v8y=\"",$dy[$j]*(0.5),"\" v7x=\"",$dx[$j]*(0.5),"\" v7y=\"",$dy[$j]*(-0.5),"\" v6x=\"",$dx[$j]*0.5,"\" v6y=\"",$dy[$j]*(-0.5),"\" dz=\"",$dx[$j]*(0.5*tan(abs($quartzCutAngle[$j]))),"\" lunit= \"mm\"/>\n";


print def "<union name=\"quartzSol_${opt_T}_$j\">
    <first ref=\"quartzRecSol_${opt_T}_$j\"/>
    <second ref=\"quartzCutSol_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"",$dx[$j]*(0.5*tan(abs($quartzCutAngle[$j])))+$dz[$j]*(0.5),"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";


print def "<arb8 name=\"refLogicSol_${opt_T}_$j\" v1x=\"",$dx[$j]*(0.5)+$width/cos($refTopOpeningAngle[$j]),"\" v1y=\"",$dy[$j]*(0.5)+$width,"\" v4x=\"",$dx[$j]*(-0.5)-$width/cos(atan(abs($dx[$j]+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]))/$dzRef[$j])),"\" v4y=\"",$dy[$j]*(0.5)+$width,"\" v3x=\"",$dx[$j]*(-0.5)-$width/cos(atan(abs($dx[$j]+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]))/$dzRef[$j])),"\" v3y=\"",$dy[$j]*(-0.5)-$width,"\" v2x=\"",$dx[$j]*(0.5)+$width/cos($refTopOpeningAngle[$j]),"\" v2y=\"",$dy[$j]*(-0.5)-$width,"\"   v5x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])+$width/cos($refTopOpeningAngle[$j]),"\" v5y=\"",$dyLg[$j]*0.5+$width,"\" v8x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j])-$width/cos(atan(abs($dx[$j]+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]))/$dzRef[$j])),"\" v8y=\"",$dyLg[$j]*(0.5)+$width,"\" v7x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j])-$width/cos(atan(abs($dx[$j]+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]))/$dzRef[$j])),"\" v7y=\"",$dyLg[$j]*(-0.5)-$width," \" v6x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])+$width/cos($refTopOpeningAngle[$j]),"\" v6y=\"",$dyLg[$j]*(-0.5)-$width,"\" dz=\"",$dzRef[$j]*(0.5),"\" lunit= \"mm\"/>\n";


print def "<arb8 name=\"lgLogicSol_${opt_T}_$j\" v1x=\"",$dzLg[$j]*0.5-$width*tan($lgTiltAngle[$j]),"\" v1y=\"",$dyLg[$j]*(0.5)+$width,"\"  v4x=\"",$dzLg[$j]*(-0.5),"\" v4y=\"",$dyLg[$j]*(0.5)+$width,"\" v3x=\"",$dzLg[$j]*(-0.5),"\" v3y=\"",$dyLg[$j]*(-0.5)-$width,"\" v2x=\"",$dzLg[$j]*0.5-$width*tan($lgTiltAngle[$j]),"\" v2y=\"",$dyLg[$j]*(-0.5)-$width,"\" v5x=\"",$dzLg[$j]*0.5+$dxLg[$j]*tan($lgTiltAngle[$j])+$width*tan($lgTiltAngle[$j]),"\" v5y=\"",$dyLg[$j]*(0.5)+$width,"\" v8x=\"",$dzLg[$j]*(-0.5),"\" v8y=\"",$dyLg[$j]*(0.5)+$width,"\" v7x=\"",$dzLg[$j]*(-0.5),"\" v7y=\"",$dyLg[$j]*(-0.5)-$width,"\" v6x=\"",$dzLg[$j]*0.5+$dxLg[$j]*tan($lgTiltAngle[$j])+$width*tan($lgTiltAngle[$j]),"\" v6y=\"",$dyLg[$j]*(-0.5)-$width,"\" dz=\"",$dxLg[$j]/2+$width,"\" lunit= \"mm\"/>\n";



print def "<union name=\"logicSol1_${opt_T}_$j\">
    <first ref=\"quartzRecSol_${opt_T}_$j\"/>
    <second ref=\"refLogicSol_${opt_T}_$j\"/>
    <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"",$dzRef[$j]*(0.5)+$dz[$j]*(0.5),"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"0\" z=\"0\"/> 
</union>\n";



print def "<union name=\"quartzLogicSol_${opt_T}_$j\">
    <first ref=\"logicSol1_${opt_T}_$j\"/>
    <second ref=\"lgLogicSol_${opt_T}_$j\"/>
   <position name=\"quartzCutPos_${opt_T}_$j\" unit=\"mm\" x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j])+$dxLg[$j]*cos($lgTiltAngle[$j])/2-$dzLg[$j]*sin($lgTiltAngle[$j])/2,"\" y=\"0\" z=\"",$dzRef[$j]+$dz[$j]*(0.5)+$dzLg[$j]*cos($lgTiltAngle[$j])/2+$dxLg[$j]*sin($lgTiltAngle[$j])/2,"\"/>
    <rotation name=\"quartzCutRot_${opt_T}_$j\" unit=\"rad\" x=\"0\" y=\"",pi/2-$lgTiltAngle[$j],"\" z=\"0\"/> 
</union>\n";




print def "<arb8 name=\"refSol_${opt_T}_$j\" v1x=\"",$dx[$j]*(0.5),"\" v1y=\"",$dy[$j]*(0.5),"\" v4x=\"",$dx[$j]*(-0.5),"\" v4y=\"",$dy[$j]*(0.5),"\" v3x=\"",$dx[$j]*(-0.5),"\" v3y=\"",$dy[$j]*(-0.5),"\" v2x=\"",$dx[$j]*(0.5),"\" v2y=\"",$dy[$j]*(-0.5),"\"   v5x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j]),"\" v5y=\"",$dyLg[$j]*0.5,"\" v8x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]),"\" v8y=\"",$dyLg[$j]*(0.5),"\" v7x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j]),"\" v7y=\"",$dyLg[$j]*(-0.5)," \" v6x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j]),"\" v6y=\"",$dyLg[$j]*(-0.5),"\" dz=\"",$dzRef[$j]*(0.5),"\" lunit= \"mm\"/>\n";

print def "<subtraction name =\"refSol1_${opt_T}_$j\">
	<first ref=\"refLogicSol_${opt_T}_$j\"/> 
	<second ref=\"refSol_${opt_T}_$j\"/> 
	<position unit=\"mm\" name=\"quartzCutPos1_${opt_T}_$j\" x=\"0\" y=\"0\" z=\"",0,"\"\/>
        <rotation unit=\"rad\" name=\"quartzCutRot1_${opt_T}_$j\" x=\"0\" y=\"0\" z=\"0\"/>
</subtraction>\n\n";

print def "<subtraction name =\"refSol2_${opt_T}_$j\">
	<first ref=\"refSol_${opt_T}_$j\"/> 
	<second ref=\"quartzCutSol_${opt_T}_$j\"/> 
	<position unit=\"mm\" name=\"quartzCut_${opt_T}_$j\" x=\"0\" y=\"0\" z=\"",$dzRef[$j]*(-0.5)+$dx[$j]*(0.5*tan(abs($quartzCutAngle[$j]))),"\"\/>
        <rotation unit=\"rad\" name=\"quartzCut_${opt_T}_$j\" x=\"0\" y=\"0\" z=\"0\"/>
</subtraction>\n\n";

print def "<arb8 name=\"lgSol_${opt_T}_$j\" v1x=\"",$dzLg[$j]*0.5,"\" v1y=\"",$dyLg[$j]*(0.5),"\"  v4x=\"",$dzLg[$j]*(-0.5),"\" v4y=\"",$dyLg[$j]*(0.5),"\" v3x=\"",$dzLg[$j]*(-0.5),"\" v3y=\"",$dyLg[$j]*(-0.5),"\" v2x=\"",$dzLg[$j]*0.5,"\" v2y=\"",$dyLg[$j]*(-0.5),"\" v5x=\"",$dzLg[$j]*0.5+$dxLg[$j]*tan($lgTiltAngle[$j]),"\" v5y=\"",$dyLg[$j]*(0.5),"\" v8x=\"",$dzLg[$j]*(-0.5),"\" v8y=\"",$dyLg[$j]*(0.5),"\" v7x=\"",$dzLg[$j]*(-0.5),"\" v7y=\"",$dyLg[$j]*(-0.5),"\" v6x=\"",$dzLg[$j]*0.5+$dxLg[$j]*tan($lgTiltAngle[$j]),"\" v6y=\"",$dyLg[$j]*(-0.5),"\" dz=\"",$dxLg[$j]/2,"\" lunit= \"mm\"/>\n";









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


print def "<volume name=\"quartzRecVol_${opt_T}\_$j\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"quartzSol_${opt_T}\_$j\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"red\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"",$index[$j],"\"/>  
</volume>\n";


print def "<volume name=\"refVol_${opt_T}\_$j\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"refSol2_${opt_T}\_$j\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"green\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"",$index[$j],"\"/>  
</volume>\n";
 
print def "<volume name=\"lgVol_${opt_T}\_$j\">
         <materialref ref=\"Quartz\"/>
         <solidref ref=\"lgSol_${opt_T}\_$j\"/> 
         <auxiliary auxtype=\"Color\" auxvalue=\"blue\"/> 
 	 <auxiliary auxtype=\"SensDet\" auxvalue=\"planeDet\"/> 
	 <auxiliary auxtype=\"DetNo\" auxvalue=\"",$index[$j],"\"/>  
</volume>\n";



print def "<volume name=\"quartzVol_${opt_T}\_$j\">
         <materialref ref=\"Air\"/>
         <solidref ref=\"quartzLogicSol_${opt_T}\_$j\"/> 
         <physvol name=\"quartzRec_${opt_T}_$j\">
			<volumeref ref=\"quartzRecVol_${opt_T}_$j\"/>
			<position name=\"detectorPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"0\"/>
			<rotation name=\"detectorRot_${opt_T}_$j\" unit=\"rad\" x=\"",0,"\" y=\"0\" z=\"0\"/>
</physvol> \n
        <physvol name=\"ref_${opt_T}_$j\">
			<volumeref ref=\"refVol_${opt_T}_$j\"/>
			<position name=\"detectorPos_${opt_T}_$j\" unit=\"mm\" x=\"0\" y=\"0\" z=\"",$dz[$j]*(0.5)+$dzRef[$j]*(0.5),"\"/>
			<rotation name=\"detectorRot_${opt_T}_$j\" unit=\"rad\" x=\"",0,"\" y=\"0\" z=\"0\"/>
</physvol> \n
        <physvol name=\"lg_${opt_T}_$j\">
			<volumeref ref=\"lgVol_${opt_T}_$j\"/>
			<position name=\"detectorPos_${opt_T}_$j\" unit=\"mm\" x=\"",$dx[$j]*(0.5)+$dzRef[$j]*tan($refTopOpeningAngle[$j])-$dxLg[$j]/cos($lgTiltAngle[$j])+$dxLg[$j]*cos($lgTiltAngle[$j])/2-$dzLg[$j]*sin($lgTiltAngle[$j])/2,"\" y=\"0\" z=\"",$dzRef[$j]+$dz[$j]*(0.5)+$dzLg[$j]*cos($lgTiltAngle[$j])/2+$dxLg[$j]*sin($lgTiltAngle[$j])/2,"\"/>
			<rotation name=\"detectorRot_${opt_T}_$j\" unit=\"rad\" x=\"",0,"\" y=\"",(-1)*pi/2+$lgTiltAngle[$j],"\" z=\"0\"/>
</physvol> \n

</volume>\n";


}
print def "<volume name=\"logicMotherVol_${opt_T}\"> 
	<materialref ref=\"Air\"/>
	<solidref ref=\"logicMotherSol_${opt_T}\"/>\n";
for $j(0..$i-1){
print def "<physvol name=\"detector_${opt_T}_$j\">
			<volumeref ref=\"quartzVol_${opt_T}_$j\"/>
			<position name=\"detectorPos_${opt_T}_$j\" unit=\"mm\" x=\"$x[$j]\" y=\"$y[$j]\" z=\"$z[$j]\"/>
			<rotation name=\"detectorRot_${opt_T}_$j\" unit=\"rad\" x=\"",$rx[$j],"\" y=\"$ry[$j]\" z=\"$rz[$j]\"/>
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




