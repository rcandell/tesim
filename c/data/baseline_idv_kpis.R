

source("multiplot.R")

library(ggplot2)

source("calc_kpis.R")

base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
xmeas_idx <- c( 5, c(1:4, 5, 7:12, 14, 15, 17, 23, 25, 40, 42) + 17)
xmv_idx <- c( 5, c(6:17))
all_disturbs <- c( 5, c(60:74, 80))
base.time <- base.df$time
base.xmeas <- base.df[xmeas_idx]
base.xmv <- base.df[xmv_idx]
base.vars <- base.df[all_disturbs]

calc_itae(data = base.xmeas, t = "time", x = "Reactor.Pressure.kPa")

