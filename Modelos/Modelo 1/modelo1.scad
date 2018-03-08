module ladron()
{
    cube([300,50,30]);
}

module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\modelos\\modulos\\rasp.stl",convexity = 3);
}
module cluster_rasp()
{
 
color("grey") 
translate([0,0,135]) 
    rasp();    
color("YellowGreen") 
translate([0,0,115]) 
    rasp();
color("ForestGreen") 
translate([0,0,95]) 
    rasp();
color("YellowGreen") 
translate([0,0,75]) 
    rasp();
color("ForestGreen") 
translate([0,0,55]) 
    rasp();
color("YellowGreen") 
translate([0,0,35]) 
    rasp();
color("ForestGreen") 
translate([0,0,15]) 
    rasp();
}


module ventilador()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\modelos\\modulos\\fan.stl",convexity = 3);
}

module switch()
{
    cube([140,90,30]);
}

module box(){

    translate([-4,-325,0])
    cube([254,350,5]);
    
 //   translate([-4,-325,0])
 //   cube([5,350,120]);
    
    translate([247,-325,0])
    cube([5,350,120]);
    
    translate([0,-325,0])
    cube([250,5,120]);

}
module box_things()
{
    rotate([0,0,90])
    translate([-320,-240,0])
    color("black")
    ladron();
    
    color("red")
    translate([0,3,0])
    ventilador();
    color("green")
    translate([125,3,0])
    ventilador();

    translate([0,-50,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([20,-320,5])
    color("RoyalBlue") 
    switch();
}

box();
box_things();


