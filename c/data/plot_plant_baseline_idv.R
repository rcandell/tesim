

library(ggplot2)

source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

plot_titles<-data.frame(n=names(base.df[xmeas_idx]),l=xmeas_labels)

# load the baseline data
base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
idv.df <- read.table("nochan/allidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
idv.df$IDV = as.factor(idv.df$IDV)

# plot histograms for reactor pressure versus idv
i="Reactor.Pressure.kPa"
ggplot() +
  geom_histogram(data = idv.df, aes(x=Reactor.Pressure.kPa, y=..density..), color="black", fill="white")  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  geom_density(data = idv.df, aes(x=Reactor.Pressure.kPa)) +
  facet_wrap(~IDV, ncol = 4, scales="free") +
  ylab(" ") + xlab("")

# add SS case to all hist plots if that might add value
base_rp<-data.frame(Reactor.Pressure.kPa=base.df$Reactor.Pressure.kPa)
ggplot() +
  geom_histogram(data = idv.df, aes(x=Reactor.Pressure.kPa, y=..density..), color="black", fill="white")  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  geom_density(data = base_rp, aes(x=Reactor.Pressure.kPa)) +
  facet_wrap(~IDV, ncol = 4, scales="free") +
  ylab(" ") + xlab("")

# TODO: add comparison denisty plot for network versus nochan cases

# plot time series for reactor pressure versus idv
ggplot(data = idv.df) +
  geom_line(aes(x=time, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  facet_wrap(~IDV, ncol = 3, scales="free") +
  ylab(" ") + xlab("time (hrs)")

ggplot(data = idv.df) +
  geom_line(aes(x=time, y=Reactor.Pressure.kPa, color=IDV, group=IDV))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  ylab(" ") + xlab("time (hrs)")

###  BOXES PLOTS 
## plot box plots for reactor pressure versus idv
ggplot(data = idv.df) +
  geom_boxplot(aes(x=IDV, y=Reactor.Pressure.kPa))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  facet_grid(~IDV, scales="free") 

## Stripper level
i<-"Stripper.Underflow.m3.hr"
ggplot(data = idv.df) +
  geom_boxplot(aes(x=IDV, y=Stripper.Underflow.m3.hr))  +
  ggtitle(plot_titles[plot_titles$n==i,]$l) +
  facet_grid(~IDV, scales="free") 

i<-"Purge.Rate.kscmh"
ggplot(data = idv.df) +
  geom_boxplot(aes(x=IDV, y=Purge.Rate.kscmh))  +
  ggtitle("Purge Flow Rate") +
  facet_grid(~IDV, scales="free") 

i<-"Hourly.Cost"
ggplot(data = idv.df) +
  geom_boxplot(aes(x=IDV, y=Hourly.Cost))  +
  ggtitle("Hourly.Cost") +
  facet_grid(~IDV, scales="free") + 
  theme(axis.ticks = element_blank(), axis.text.x = element_blank())

