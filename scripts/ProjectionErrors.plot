set style fill empty
set logscale y
set xrange[0.8:9]
set boxwidth 0.05
set xtics rotate
set ylabel "Distance Difference (Meters)"
set yrange[0.01:]
set term png
set output "tmp/DistanceErrors.png"
plot 'tmp/SmallNorth.dat' using 1:4:3:7:6:xticlabels(2) with candlesticks lt rgb 'blue' lw 2 title 'Small North' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/BigNorth.dat'   using 1:4:3:7:6 with candlesticks lt rgb 'red' lw 2 title 'Big North' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/ReasonableNorth.dat'   using 1:4:3:7:6 with candlesticks lt rgb 'green' lw 2 title 'Reasonable North' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/AlmostGlobe.dat'   using 1:4:3:7:6 with candlesticks lt rgb '#D2691E' lw 2 title 'Almost Globe' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/ReasonableGlobe.dat'   using 1:4:3:7:6 with candlesticks lt rgb 'cyan' lw 2 title 'Reasonable Globe' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/SmallMiddle.dat'   using 1:4:3:7:6 with candlesticks lt rgb 'purple' lw 2 title 'Small Middle' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle, \
     'tmp/MediumMiddle.dat'   using 1:4:3:7:6 with candlesticks lt rgb '#20B2AA' lw 2 title 'Medium Middle' whiskerbars, \
     ''                   using 1:5:5:5:5 with candlesticks lt -1 lw 2 notitle

set nologscale y
set ylabel "Angle Difference (Degrees)"
set yrange[0:180]
set term png
set output "tmp/AngleErrors.png"
plot 'tmp/SmallNorth.dat' using 1:9:8:12:11:xticlabels(2) with candlesticks lt rgb 'blue' lw 2 title 'Small North' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/BigNorth.dat'   using 1:9:8:12:11 with candlesticks lt rgb 'red' lw 2 title 'Big North' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/ReasonableNorth.dat'   using 1:9:8:12:11 with candlesticks lt rgb 'green' lw 2 title 'Reasonable North' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/AlmostGlobe.dat'   using 1:9:8:12:11 with candlesticks lt rgb '#D2691E' lw 2 title 'Almost Globe' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/ReasonableGlobe.dat'   using 1:9:8:12:11 with candlesticks lt rgb 'cyan' lw 2 title 'Reasonable Globe' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/SmallMiddle.dat'   using 1:9:8:12:11 with candlesticks lt rgb 'purple' lw 2 title 'Small Middle' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle, \
     'tmp/MediumMiddle.dat'   using 1:9:8:12:11 with candlesticks lt rgb '#20B2AA' lw 2 title 'Medium Middle' whiskerbars, \
     ''                   using 1:10:10:10:10 with candlesticks lt -1 lw 2 notitle

