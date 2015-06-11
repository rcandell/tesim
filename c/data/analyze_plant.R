

library(ggplot2)

source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

# load the baseline data
base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

base.time <- base.df$time
base.xmeas <- base.df[xmeas_idx]
base.xmeas$time <- base.time


print_to_file<-TRUE

# FANCY TITLES
plot_titles<-data.frame()
xmeas_names<-names(base.xmeas)[1:length(names(base.xmeas))-1]
plot_titles<-data.frame(cbind(n=xmeas_names, l=xmeas_labels), stringsAsFactors = F)

## TIME SERIES
ts_plots<-c()
for (i in names(base.xmeas))
{
  if(i!="time")
  {
    print(i)
    ts_plots[[i]]<-ggplot(data = base.xmeas) +
      geom_line(aes_string(x="time", y=i))  +
      ggtitle(plot_titles[plot_titles$n==i,]$l) +
      ylab(" ") + xlab("time (hrs)")
  }
}

## HISTOGRAMS
hist_plots<-c()
for (i in names(base.xmeas))
{
  if(i!="time")
  {
    print(i)
    w<-range(base.xmeas[i])[2]/10000
    hist_plots[[i]]<-ggplot(data = base.xmeas) +
      geom_histogram(aes_string(x=i), binwidth=w, color="black", fill="white")  +
      ggtitle(plot_titles[plot_titles$n==i,]$l) +
      ylab(" ") + xlab("time (hrs)")
  }
}

if (print_to_file)
{
  png(filename = "tseries-xmeas.png", pointsize=8, bg = "transparent", res = NA, restoreConsole = TRUE)
  multiplot(plotlist = ts_plots, cols=2)
  dev.off()
  
  png(filename = "histograms-xmeas.png", pointsize=8, bg = "transparent", res = NA, restoreConsole = TRUE)
  multiplot(plotlist = hist_plots, cols=2)
  dev.off()
  
} else
{
  multiplot(plotlist = ts_plots, cols=2)
  multiplot(plotlist = hist_plots, cols=2)
}


