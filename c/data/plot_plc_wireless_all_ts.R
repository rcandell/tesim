
library(ggplot2)
library(reshape2)

#source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

loc_baseline<-"Y:/tesim/c/data/baseline"
loc_experimental<-"Y:/tesim/c/data/with_antenna.cal"
#loc_experimental<-"Y:/tesim/c/data/with_antenna"
#prefixes<-c("Lreactor_80","Preactor2700","Prodrate25.0","Pctg_35","ReactorCoolingStuck")
#grp_titles<-c("Reactor Level Change to 80%","Reactor Pressure Change to 2700 kPa","Production Rate Change to 25","% G Change to 35%","Reactor Cooling Valve Stuck")
prefixes<-c("Preactor2700")
grp_titles<-c("Reactor Pressure Change to 2700 kPa")
dlist <- cbind(dfp=prefixes,gt=grp_titles)

plot_ts <- function (X, loc_base, loc_exp, melt_vars, ptype=1, save=F, ht=8.0, wd=5.5)
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
  d_err$y_pct <- 100*(d_err$y/d_base$y)
  
  # just print the stats
  if(ptype == 0)
  {
    #g <- format(aggregate(d_err$y_pct, by=list(d_err$x), FUN=summary), digits=3, scientific=F)
    g<-aggregate(d_err$y_pct, by=list(d_err$x), FUN=min)
    names(g)<-c("Variable","min")
    g$max<-aggregate(d_err$y_pct, by=list(d_err$x), FUN=max)$x
    g$mean<-aggregate(d_err$y_pct, by=list(d_err$x), FUN=mean)$x
    g$sd<-aggregate(d_err$y_pct, by=list(d_err$x), FUN=sd)$x
    print(g)
    write.table(format(g, digits=1, scientific=F), file=paste0(X[["dfp"]],"_stats.txt"), sep="\t", quote = F, row.names = F)  
  }
  #
  # plot time series of baseline and experimental
  #
  else if(ptype == 1)
  {
    g<-ggplot(data=d_all) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_line(aes(x=time, y=y, colour=run_type), legend.position = "bottom") +
      theme(legend.position="bottom")
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_ts.png"), scale=1.0, height = ht, width = wd) 
    }
  }
  
  #
  # deviations from baseline
  #
  else if(ptype == 2) 
  {
    # plot time series of deviations
    g<-ggplot(data=d_err) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_line(aes(x=time, y=y)) +
      ggtitle(paste(X[["gt"]],"-- Deviation from Baseline"))
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_err.png"), scale=1.0, height = ht, width = wd) 
    }
    print(g)
    
    # plot density of deviations
    g<-ggplot(data=d_err) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_density(aes(x=y)) +
      ggtitle(paste(X[["gt"]],"-- Deviation from Baseline"))
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_errden.png"), scale=1.0, height = ht, width = wd) 
    }    
    
    # plot density of percentage deviations
    g<-ggplot(data=d_err) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_density(aes(x=y_pct)) +
      ggtitle(paste(X[["gt"]],"-- % Deviation from Baseline"))
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_pcterrden.png"), scale=1.0, height = ht, width = wd) 
    }        
    
    # plot box plots of deviations
    g<-ggplot(data = d_err) +
      theme_bw() + 
      geom_boxplot(aes(x=x, y=y_pct))  +
      scale_x_discrete(labels=xmeas_labels_abbr) +
      ggtitle(paste(X[["gt"]],"-- % Deviation from Baseline"))
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_pcterrbox.png"), scale=1.0, height = ht, width = wd) 
    }    
  }
  
  #
  # Histograms of baseline versus experimental
  #
  else if(ptype == 3)
  {
    # Plot histograms of baseline and experimental 
    g<-ggplot(data=d_all) + 
      theme_bw() + 
      ggtitle(paste(X[["gt"]],"\n")) +
      facet_wrap(~x, ncol=3, shrink=T, scales = "free") + 
      geom_density(aes(x=y, colour=run_type))
    print(g)
    
    if(save) 
    { 
      ggsave(paste0(X[["dfp"]],"_den.png"), scale=1.0, height = ht, width = wd) 
    }
  }

}


if (length(dev.list()["RStudioGD"])) {dev.off(dev.list()["RStudioGD"])}

for( a_ptype in 1)
{
  a_save = T
  apply( X=dlist, 1, FUN=plot_ts, 
         loc_base=loc_baseline, 
         loc_exp=loc_experimental,
         melt_vars=xmeas_melt_vars, ptype=a_ptype, save=a_save, ht = 5.5, wd = 7.5 )

}


