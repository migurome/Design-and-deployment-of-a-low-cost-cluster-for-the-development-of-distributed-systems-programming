module ladron()
{
    cube([300,50,30]);
}

module rasp()
{
    color("Grey") 
    import("C:\\Users\\null\\Desktop\\RickAndMortys\\modelos\\modulos\\rasp.stl",convexity = 3);
}

module flecha(){
    
    
        rotate([0,45,0])

    polyhedron ( points = [[0, -10, 60], [0, 10, 60], [0, 10, 0], [0, -10, 0], [60, -10, 60], [60, 10, 60]], 
triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
    
    

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

    //color("Gainsboro")
    //translate([-4,-325,0])
    //cube([5,350,120]);
    
    
    color("Gainsboro")
    translate([247,-325,0])
    cube([5,350,120]);
    color("Gainsboro")
    translate([0,-325,0])
    cube([250,5,120]);

}

module box_things()
{
    rotate([0,0,90])
    translate([-320,-240,0])
    color("IndianRed")
    ladron();
    
    color("LightSlateGray")
    translate([0,3,0])
    ventilador();
    color("LightSlateGray")
    translate([125,3,0])
    ventilador();

    translate([0,-50,30])
    rotate([90,0,90])
    cluster_rasp();
    
    translate([20,-320,5])
    color("SteelBlue") 
    switch();
    
    color("Lime")
    translate([18,90,63])
    rotate([90,0,0])
    flecha();
        
    color("Red")
    translate([150,47,63])
    rotate([270,0,0])
    flecha();
}


module caja(){
box_things();
box();
}

rotate([0,0,1000* $t])

translate([-120,140,0])
caja();


