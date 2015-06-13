

library(ggplot2)

source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

# load the baseline data
base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

# plot histograms for reactor pressure versus idv
plot_titles<-data.frame(n=names(base.df[xmeas_idx]),l=xmeas_labels)
i="Reactor.Pressure.kPa"
ggplot(data = base.df) +
  geom_histogram(aes(x=Reactor.Pressure.kPa, y=..density..), color="black", fill="white")  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  geom_density(aes(x=Reactor.Pressure.kPa)) + 
  ylab(" ") + xlab("time (hrs)")

# plot time series for reactor pressure versus idv
ggplot(data = base.df) +
  geom_line(aes(x=time, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  ylab(" ") + xlab("time (hrs)")

###  BOXES PLOTS 
## plot box plots for reactor pressure versus idv
ggplot(data = base.df) +
  geom_boxplot(aes(x=time, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l)

## Stripper level
i<-"Stripper.Underflow.m3.hr"
ggplot(data = base.df) +
  geom_boxplot(aes(x=time, y=Stripper.Underflow.m3.hr))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) 

i<-"Purge.Rate.kscmh"
ggplot(data = base.df) +
  geom_boxplot(aes(x=time, y=Purge.Rate.kscmh))  +
  ggtitle("Purge Flow Rate") 
