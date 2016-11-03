import logging
import logging.config

if __name__=="__main__":
    logging.config.fileConfig("server.conf")

    logger = logging.getLogger('network')
    logger.debug("debug data")
    logger.info("info data")
    logger.warn("warning data")
    logger.error("Error message")
    logger.critical("Critical message")
