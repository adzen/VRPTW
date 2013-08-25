VRPTW
=====

"Vehicle Routing Problem with Time Windows"

## Problem Description

  Given a depot with homogeneous fleet and a set of customers to be served.
  
  Find a set of closed routes (start from depot, end at depot) to serve all customers with no exceeding vehicle's capacity and customers' due time.
  
  Use as less number of vehicle and shorter total travel distance as possible.

## Benchmark Problem Sets 

 * [Solomon's problem sets] (http://w.cba.neu.edu/~msolomon/problems.htm) (25, 50, and 100 customers)
 * [Gehring & Homberger's extended benchmark] (http://www.sintef.no/Projectweb/TOP/VRPTW/Homberger-benchmark/) (200, 400, 600, 800, and 1000 customers)

## Best Known Solutions

 * [VRPTW best solutions for Solomon's benchmarks] (http://sun.aei.polsl.pl/~zjc/best-solutions-solomon.html) (from Zbigniew J. Czech, 100 customers only)
 * [VRPTW best solutions for Gehring and Homberger's benchmarks] (http://sun.aei.polsl.pl/~zjc/BestSolutionsGH.html) (from Zbigniew J. Czech)

## Input Format

    vehicle_capacity
    number_of_customers
     
  Remaining lines are each customer's information:
  
    id  x_pos  y_pos  demand  ready_time  due_time  unloading_time
     
  
   
