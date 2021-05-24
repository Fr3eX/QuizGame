-- MySQL dump 10.13  Distrib 8.0.22, for Linux (x86_64)
--
-- Host: localhost    Database: QUIZ_GAME
-- ------------------------------------------------------
-- Server version	8.0.25-0ubuntu0.20.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;



CREATE DATABASE QUIZ_GAME;

--
-- Table structure for table `Q_U_STATUS`
--

DROP TABLE IF EXISTS `Q_U_STATUS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Q_U_STATUS` (
  `userId` int NOT NULL,
  `quizId` int NOT NULL,
  `mark` int DEFAULT NULL,
  PRIMARY KEY (`userId`,`quizId`),
  KEY `quizId` (`quizId`),
  CONSTRAINT `Q_U_STATUS_ibfk_1` FOREIGN KEY (`userId`) REFERENCES `User` (`id`),
  CONSTRAINT `Q_U_STATUS_ibfk_2` FOREIGN KEY (`quizId`) REFERENCES `Quiz` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Q_U_STATUS`
--

LOCK TABLES `Q_U_STATUS` WRITE;
/*!40000 ALTER TABLE `Q_U_STATUS` DISABLE KEYS */;
/*!40000 ALTER TABLE `Q_U_STATUS` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Quiz`
--

DROP TABLE IF EXISTS `Quiz`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Quiz` (
  `id` int NOT NULL AUTO_INCREMENT,
  `quizOwner` int DEFAULT NULL,
  `quizCode` int DEFAULT NULL,
  `NBR_QP` int DEFAULT NULL,
  `NBR_QPS` int DEFAULT NULL,
  `NBR_QPF` int DEFAULT NULL,
  `startTime` datetime DEFAULT NULL,
  `deadTime` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `quizOwner` (`quizOwner`),
  CONSTRAINT `Quiz_ibfk_1` FOREIGN KEY (`quizOwner`) REFERENCES `User` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Quiz`
--

LOCK TABLES `Quiz` WRITE;
/*!40000 ALTER TABLE `Quiz` DISABLE KEYS */;
/*!40000 ALTER TABLE `Quiz` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Quiz_q`
--

DROP TABLE IF EXISTS `Quiz_q`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Quiz_q` (
  `id` int NOT NULL AUTO_INCREMENT,
  `quizId` int DEFAULT NULL,
  `mark` int DEFAULT NULL,
  `question` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `quizId` (`quizId`),
  CONSTRAINT `Quiz_q_ibfk_1` FOREIGN KEY (`quizId`) REFERENCES `Quiz` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Quiz_q`
--

LOCK TABLES `Quiz_q` WRITE;
/*!40000 ALTER TABLE `Quiz_q` DISABLE KEYS */;
/*!40000 ALTER TABLE `Quiz_q` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Quiz_r`
--

DROP TABLE IF EXISTS `Quiz_r`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Quiz_r` (
  `id` int NOT NULL AUTO_INCREMENT,
  `quiz_qId` int DEFAULT NULL,
  `isCorrect` tinyint(1) DEFAULT NULL,
  `answer` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `quiz_qId` (`quiz_qId`),
  CONSTRAINT `Quiz_r_ibfk_1` FOREIGN KEY (`quiz_qId`) REFERENCES `Quiz_q` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Quiz_r`
--

LOCK TABLES `Quiz_r` WRITE;
/*!40000 ALTER TABLE `Quiz_r` DISABLE KEYS */;
/*!40000 ALTER TABLE `Quiz_r` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User`
--

DROP TABLE IF EXISTS `User`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `User` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(64) DEFAULT NULL,
  `lastIp` varchar(20) DEFAULT NULL,
  `NBR_Q` int DEFAULT NULL,
  `NBR_QP` int DEFAULT NULL,
  `NBR_QPS` int DEFAULT NULL,
  `NBR_QPF` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User`
--

LOCK TABLES `User` WRITE;
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
/*!40000 ALTER TABLE `User` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-05-24 19:56:31
