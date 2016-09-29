#!/usr/bin/perl -w

# use perl open function to open the file (or die trying)

open(FILE2, "mirrorTranslations.txt") or die "Could not read from $filename, program halting.";

# loop through the file1 with a perl while loop, stopping when you get to the desired record
my @replace;
while(<FILE2>)
{
    push(@replace, $_);
    last if $. == 188;
}
close FILE2;

open(FILE1, "qsimDetectorConstruction.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 0;
$start = 90 - $index;
my @newlines1;
while (<FILE1>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines1,$_);
	last if $. == 1204;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines1,$replace[$index]);
	if($index < 3) {$index++;}
    }
    $count1++;
}
close FILE1;

open(FILE3, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE3 @newlines1;
close FILE3;

open(FILE4, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 4;
$start = 390 - $index;
my @newlines2;
while (<FILE4>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines2,$_);
	last if $. == 1204;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines2,$replace[$index]);
	if($index < 14) {$index++;}
    }
    $count1++;
}
close FILE4;

open(FILE5, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE5 @newlines2;
close FILE5;

open(FILE6, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 15;
$start = 540 - $index;
my @newlines3;
while (<FILE6>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines3,$_);
	last if $. == 1220;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines3,$replace[$index]);
	if($index < 45) {$index++;}
    }
    $count1++;
}
close FILE6;

open(FILE7, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE7 @newlines3;
close FILE7;

open(FILE8, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 46;
$start = 596 - $index;
my @newlines4;
while (<FILE8>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines4,$_);
	last if $. == 1220;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines4,$replace[$index]);
	if($index < 64) {$index++;}
    }
    $count1++;
}
close FILE8;

open(FILE9, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE9 @newlines4;
close FILE9;

open(FILE10, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 65;
$start = 860 - $index;
my @newlines5;
while (<FILE10>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines5,$_);
	last if $. == 1220;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines5,$replace[$index]);
	if($index < 143) {$index++;}
    }
    $count1++;
}
close FILE10;

open(FILE11, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE11 @newlines5;
close FILE11;

open(FILE12, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 144;
$start = 993 - $index;
my @newlines6;
while (<FILE12>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines6,$_);
	last if $. == 1220;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines6,$replace[$index]);
	if($index < 185) {$index++;}
    }
    $count1++;
}
close FILE12;

open(FILE13, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE13 @newlines6;
close FILE13;


open(FILE14, "qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
$count1 = 1;
$index = 186;
$start = 1046 - $index;
my @newlines7;
while (<FILE14>)
{
    $count = $start + $index;
    if($count1 != $count)
    {
	push(@newlines7,$_);
	last if $. == 1220;
    } else
    {
	print $count, $_, $replace[$index];
	#chomp $replace[$index];
	push(@newlines7,$replace[$index]);
	if($index < 187) {$index++;}
    }
    $count1++;
}
close FILE14;

open(FILE15, ">qsimDetectorConstructionNew.cc") or die "Could not read from $filename, program halting.";
print FILE15 @newlines7;
close FILE15;
