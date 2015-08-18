
library(ggplot2)
library(reshape2)

#source("multiplot.R")

setwd("C:/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

loc_baseline<-"C:/git/tesim/c/data/baseline"
loc_experimental<-"C:/git/tesim/c/data/with_antenna"
prefixes<-c("Lreactor_80","Preactor2700","Prodrate25.0","Pctg_35","ReactorCoolingStuck")
grp_titles<-c("Reactor Level Change to 80%","Reactor Pressure Change to 2700 kPa","Production Rate Change to 25","% G Change to 35%","Reactor Cooling Valve Stuck")
dlist <- cbind(dfp=prefixes,gt=grp_titles)

plot_ts <- function (X, loc_base, loc_exp, melt_vars, ptype=1)
{
  base_path <- paste0(loc_base, "/", X["dfp"], "_plant.dat")
  exp_path <- paste0(loc_exp, "/", X["dfp"], "_plant.dat")
  
  # load baseline data
  local.base <- read.table(base_path, header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
  d_base <- melt(local.base[c("time",melt_vars)], id.vars = "time", 
           variable.name = "x", value.name = "y")
  d_base$run_type <- rep("base",nrow(d_base))
  
  # load experimental data
  local.exp <- read.table(exp_path, header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
  d_exp <- melt(local.exp[c("time",melt_vars)], id.vars = "time", 
                 variable.name = "x", value.name = "y")
  d_exp$run_type <- rep("exp",nrow(d_exp))

  # join the data into one melt  
  d_all <- rbind(d_base, d_exp)

  # calculate error vector between baseline and experimental
  d_err <- d_base
  d_err$y <- d_exp$y - d_base$y
  
  # plot time series of baseline and experimental
  if(ptype == 1)
  {
    ggplot(data=d_all) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_line(aes(x=time, y=y, colour=run_type))
    
    ggsave(paste0(X[["dfp"]],"_ts.png"), scale=1.0)
  }  
  else if(ptype == 2)
  {
    # plot time series of deviations
    ggplot(data=d_err) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_line(aes(x=time, y=y)) +
      ggtitle(paste(X[["gt"]]," Absolute errors"))
    
    ggsave(paste0(X[["dfp"]],"_err.png"), scale=1.0)
  }
  else if(ptype == 3)
  {
    # Plot histograms of baseline and experimental 
    ggplot(data=d_all) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_density(aes(x=y, colour=run_type)) +
      ggsave(paste0(X[["dfp"]],"_den.png"), scale=1.0)
  }
  
}

if (length(dev.list()["RStudioGD"])) {dev.off(dev.list()["RStudioGD"])}

apply( X=dlist, 1, FUN=plot_ts, 
       loc_base=loc_baseline, 
       loc_exp=loc_experimental,
       melt_vars=xmeas_melt_vars, ptype=1 )


apply( X=dlist, 1, FUN=plot_ts, 
       loc_base=loc_baseline, 
       loc_exp=loc_experimental,
       melt_vars=xmeas_melt_vars, ptype=2 )


apply( X=dlist, 1, FUN=plot_ts, 
       loc_base=loc_baseline, 
       loc_exp=loc_experimental,
       melt_vars=xmeas_melt_vars, ptype=3 )



