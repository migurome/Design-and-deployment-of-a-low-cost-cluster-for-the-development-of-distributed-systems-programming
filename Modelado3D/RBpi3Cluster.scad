/* (c) 2016++ by Saarbastler 
**  https://github.com/saarbastler/library.scad
**
** Raspberry PI model library for generating cases etc.
*/
$fn=100;
// Show Header
header = true; // true:Show Header;false:Don't show Header

// Header Up/Down for Pi Zero
headerDown = false; //true: Header down (Only Zero): false Header up

module header(pins, rows)
{
  color("darkgrey") cube([2.54*pins,2.54*rows,1.27]);
  
  for(x=[0:pins-1],y=[0:rows-1])
    translate([x*2.54+(1.27+.6)/2,y*2.54+(1.27+.6)/2,-3.5]) cube([0.6,0.6,11.5]);
}

module pi3()
{
  // PCB
  color("limegreen") difference()
  {
    hull()
    {
      translate([-(85-6)/2,-(56-6)/2,0]) cylinder(r=3, h=1.4 );
      translate([-(85-6)/2, (56-6)/2,0]) cylinder(r=3, h=1.4 );
      translate([ (85-6)/2,-(56-6)/2,0]) cylinder(r=3, h=1.4 );
      translate([ (85-6)/2, (56-6)/2,0]) cylinder(r=3, h=1.4 );
    }
    
    translate([-85/2+3.5,-49/2,-1]) cylinder(d=2.75, h=3);
    translate([-85/2+3.5, 49/2,-1]) cylinder(d=2.75, h=3);
    translate([58-85/2+3.5,-49/2,-1]) cylinder(d=2.75, h=3);
    translate([58-85/2+3.5, 49/2,-1]) cylinder(d=2.75, h=3);
  }
  
  // Header
  translate([3.5-85/2+29-10*2.54,49/2-2.54,1.4])
    header(20,2);
  
  translate([-85/2,-56/2,1.4])  
  {
    color("silver") 
    {
      // Ethernet
      translate([85-19,11.5-16/2,0]) cube([21,16,13.8]);
    
      // USB
      translate([85-15, 29-13/2,0]) cube([17,13,15.5]);
      translate([85-15, 47-13/2,0]) cube([17,13,15.5]);
      
      // micro USB
      translate([10.6-8/2,-1.5,0]) cube([8,6,2.6]);
      
      // HDMI
      translate([32-15/2,-1.5,0]) cube([15,11.5,6.6]);
    }
    
    color("darkgrey") 
    {
      // Audio
      translate([53.5-7/2,-2,0]) 
      {
        translate([0,2,0]) cube([7,13,5.6]);
        translate([7/2,0,5.6/2])rotate([-90,0,0]) cylinder(d=5.6,h=2);
      }
    
      // Display
      translate([1.1,(49-22)/2,0]) cube([4,22,5.5]);
      
      // Camera
      translate([45-4/2,1.1,0]) cube([4,22,5.5]);
    }
    
    // Micro SD Card
    color("silver") translate([0,22,-2.9]) cube([13,14,1.5]);    
    color("darkgrey") translate([-2.4,23.5,-2.65]) cube([2.4,11,1]);
  }
}

translate([0,0,100]) 
pi3();
translate([0,0,80]) 
pi3();
translate([0,0,60]) 
pi3();
translate([0,0,40]) 
pi3();
translate([0,0,20]) 
pi3();
pi3();