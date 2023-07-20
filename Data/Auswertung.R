library(psych)
library(mosaic)

#--- just look for some KPIs -----------------------------------------
results <- read.csv2("results.csv", sep = ";", dec = ",")
speed <- read.csv2("speed.csv", sep = ";", dec = ",")
quality <- read.csv2("quality.csv", sep = ";", dec = ",")

# ---- Quality - Bootstrapping ---------------------------------

## ---- QUADPOS --------
meanQuadPos <- mean(quality$QUADPOS, na.rm=TRUE)

## ---- GREEDY vs. QUADPOS --------
meanGREEDY <- mean(quality$GREEDY, na.rm=TRUE)

set.seed(823)
bootvtlgGreedy <- do(10000) * mean(~GREEDY, data=resample(quality), na.rm=TRUE)
lowerbootvtlgGreedy <- do(5000) * mean(~GREEDY, data=resample(quality), na.rm=TRUE)

# Normalverteilung?
ks.test(bootvtlgGreedy$mean, "pnorm", mean=mean(bootvtlgGreedy$mean), sd=sd(bootvtlgGreedy$mean))
shapiro.test(lowerbootvtlgGreedy$mean)

write.csv2(bootvtlgGreedy, file="histogramQualityGreedy.csv", sep=";")

qplot(bootvtlgGreedy$mean,
      geom = "histogram")

quantile(~mean, data=bootvtlgGreedy, probs=c(0.0, 0.95))

# GG
prop(~mean > meanQuadPos, data=bootvtlgGreedy)

## ---- FORCE vs. QUADPOS --------
meanFORCE <- mean(quality$FORCE, na.rm=TRUE)

set.seed(132)
bootvtlgFORCE <- do(10000) * mean(~FORCE, data=resample(quality), na.rm=TRUE)
lowerbootvtlgFORCE<- do(5000) * mean(~FORCE, data=resample(quality), na.rm=TRUE)

# Normalverteilung?
ks.test(bootvtlgFORCE$mean, "pnorm", mean=mean(bootvtlgFORCE$mean), sd=sd(bootvtlgFORCE$mean))
shapiro.test(lowerbootvtlgFORCE$mean)

write.csv2(bootvtlgFORCE, file="histogramQualityForce.csv", sep=";")

qplot(bootvtlgFORCE$mean,
      geom = "histogram")

quantile(~mean, data=bootvtlgFORCE, probs=c(0.0, 0.95))

# GG
prop(~mean > meanQuadPos, data=bootvtlgFORCE)

## ---- SEQUENCE vs. QUADPOS --------
meanSEQUENCE <- mean(quality$SEQUENCE_MONTE_CARLO, na.rm=TRUE)

set.seed(132)
bootvtlgSEQUENCE <- do(10000) * mean(~SEQUENCE_MONTE_CARLO, data=resample(quality), na.rm=TRUE)
lowerbootvtlgSEQUENCE<- do(5000) * mean(~SEQUENCE_MONTE_CARLO, data=resample(quality), na.rm=TRUE)

# Normalverteilung?
ks.test(bootvtlgSEQUENCE$mean, "pnorm", mean=mean(bootvtlgSEQUENCE$mean), sd=sd(bootvtlgSEQUENCE$mean))
shapiro.test(lowerbootvtlgSEQUENCE$mean)

write.csv2(bootvtlgSEQUENCE, file="histogramQualitySEQUENCE.csv", sep=";")

qplot(bootvtlgSEQUENCE$mean,
      geom = "histogram")

quantile(~mean, data=bootvtlgSEQUENCE, probs=c(0.0, 0.95))

# GG
prop(~mean > meanQuadPos, data=bootvtlgSEQUENCE)

## ---- ROLLOUT vs. QUADPOS --------
meanROLLOUT <- mean(quality$ROLLOUT_MONTE_CARLO, na.rm=TRUE)

set.seed(132)
bootvtlgROLLOUT <- do(10000) * mean(~ROLLOUT_MONTE_CARLO, data=resample(quality), na.rm=TRUE)
lowerbootvtlgROLLOUT<- do(5000) * mean(~ROLLOUT_MONTE_CARLO, data=resample(quality), na.rm=TRUE)

# Normalverteilung?
ks.test(bootvtlgROLLOUT$mean, "pnorm", mean=mean(bootvtlgROLLOUT$mean), sd=sd(bootvtlgROLLOUT$mean))
shapiro.test(lowerbootvtlgROLLOUT$mean)

write.csv2(bootvtlgROLLOUT, file="histogramQualityROLLOUT.csv", sep=";")

qplot(bootvtlgROLLOUT$mean,
      geom = "histogram")

quantile(~mean, data=bootvtlgROLLOUT, probs=c(0.0, 0.95))

# GG
prop(~mean > meanQuadPos, data=bootvtlgROLLOUT)



##### ----------------------------------------------------------------
##### ---------------- SPEED -----------------------------------------
##### ----------------------------------------------------------------

#--- Speed - bootstrapping -----------------------------------------------------
meanQuadPos <- mean(speed$QUADPOS, na.rm=TRUE)
meanGREEDY <- mean(speed$GREEDY, na.rm=TRUE)

set.seed(823)
bootvtlgGreedy <- do(10000) * mean(~GREEDY, data=resample(speed), na.rm=TRUE)
lowerbootvtlgGreedy <- do(5000) * mean(~GREEDY, data=resample(speed), na.rm=TRUE)

# Kann kein Hypothesentesten machen, da keine Normalverteilung vorhanden...
ks.test(bootvtlgGreedy$mean, "pnorm", mean=mean(bootvtlgGreedy$mean), sd=sd(bootvtlgGreedy$mean))
shapiro.test(lowerbootvtlgGreedy$mean)

write.csv2(bootvtlgGreedy, file="histogramSpeed.csv", sep=";")

qplot(bootvtlgGreedy$mean,
      geom = "histogram")

quantile(~mean, data=bootvtlgGreedy, probs=c(0.0, 0.95))


# Kann kein Hypothesentesten machen, da keine Normalverteilung vorhanden...
prop(~mean > meanQuadPos, data=bootvtlgGreedy)

# ---- Speed - Mann-Whitney-U-Test ---------------------------------

quadGreedy <- results[results$Algorithmus == "GUI_ID_QUADSEARCH" | results$Algorithmus == "GUI_ID_GREEDY",]

boxplot(quadGreedy$Zeit.ms., quadGreedy$Algorithmus)

kruskal.test(quadGreedy$Zeit.ms.~quadGreedy$Algorithmus)

# Es liegen keine unterschiede vor...
wilcox.test(Zeit.ms.~Algorithmus, data = quadGreedy, exact=FALSE, correct = FALSE, conf.int = TRUE)

