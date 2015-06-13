

library(ggplot2)

source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

# load the baseline data
base.df <- read.table("nochan/allidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

# plot histograms for reactor pressure versus idv
ggplot(data = base.df) +
  geom_histogram(aes(x=Reactor.Pressure.kPa), color="black", fill="white")  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  geom_density(aes(x=Reactor.Pressure.kPa)) + 
  facet_wrap(~IDV, ncol = 2, scales="free") +
  ylab(" ") + xlab("time (hrs)")

# plot time series for reactor pressure versus idv
ggplot(data = base.df) +
  geom_line(aes(x=time, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  facet_wrap(~IDV, ncol = 3, scales="free") +
  ylab(" ") + xlab("time (hrs)")


# plot box plots for reactor pressure versus idv
ggplot(data = base.df) +
  geom_boxplot(aes(x=time, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  facet_grid(~IDV, scales="free") 

