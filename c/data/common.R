

xmeas_idx <- c(c(7,8,9,12,15,17,40,42) + 17)
xmv_idx <- c(6:17)

xmeas_labels <- c("P-reactor",
                  "L-reactor",
                  "T-reactor",
                  "L-separator",
                  "L-stripper",
                  "Production m3/hr",
                  "Quality % G mol",
                  "Hourly Cost")

xmeas_melt_vars <- c(
  "Reactor.Pressure.kPa","Reactor.Level.Pct","Reactor.Temp.C",
  "Sep.Level.Pct",
  "Stripper.Level.Pct",
  "Sep.Underflow.m3.hr",
  "Product.G.mole.Pct",
  "Hourly.Cost")
