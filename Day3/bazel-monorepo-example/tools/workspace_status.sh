#!/bin/bash
echo "BUILD_TIMESTAMP $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "GIT_COMMIT $(git rev-parse HEAD 2>/dev/null || echo unknown)"
