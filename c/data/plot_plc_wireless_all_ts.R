
library(ggplot2)
library(reshape2)

#source("multiplot.R")

setwd("C:/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

# load the plant data
#base.df <- read.table("C:/git/tesim/c/data/with_antenna/Lreactor80.1_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
#base.df <- read.table("C:/git/tesim/c/data/with_antenna/pctg_35_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
#base.df <- read.table("C:/git/tesim/c/data/with_antenna/Lreactor_80_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
base.df <- read.table("C:/git/tesim/c/data/with_antenna/Preactor2700_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

d <- melt(base.df[c("time",xmeas_melt_vars)], id.vars = "time", 
          variable.name = "x", value.name = "y")

dev.off(dev.list()["RStudioGD"])

# Everything on the same plot
ggplot(data = d, aes(x=time, y=y)) +
  facet_wrap(~x, ncol=3, shrink=T, scales = "free") +
  geom_line() +
  ggtitle("Time series for measured variables")
