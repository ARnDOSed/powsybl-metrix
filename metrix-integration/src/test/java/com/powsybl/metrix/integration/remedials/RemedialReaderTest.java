package com.powsybl.metrix.integration.remedials;

import org.apache.commons.io.FileUtils;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;

public class RemedialReaderTest {

    @Test
    public void parseFileTest() throws URISyntaxException, IOException {
        File file = new File(getClass().getResource("/remedial.txt").toURI());
        String s = FileUtils.readFileToString(file, StandardCharsets.UTF_8);
        List<Remedial> remedials = RemedialReader.parseFile(s);
        List<Remedial> expectedList = Arrays.asList(
                new Remedial(2, "S_SO_1", Collections.emptyList(), Collections.singletonList("SOO1_SOO1_DJ_OMN"), Collections.emptyList(), "SOO1_SOO1_DJ_OMN"),
                new Remedial(3, "S_SO_1", Collections.emptyList(), Arrays.asList("SS1_SS1_DJ_OMN", "AUTRE"), Arrays.asList("SOO1_SOO1_DJ_OMN", "test"), "SS1_SS1_DJ_OMN;+SOO1_SOO1_DJ_OMN;AUTRE;+test"),
                new Remedial(4, "S_SO_2", Arrays.asList("contrainte_1", "contrainte_2"), Collections.singletonList("S_SO_3"), Collections.emptyList(), "S_SO_3")
        );
        assertThat(remedials.size()).isEqualTo(expectedList.size());
        for (int i = 0; i < expectedList.size(); i++) {
            Remedial actual = remedials.get(i);
            Remedial expected = expectedList.get(i);
            assertThat(actual.getLineFile()).isEqualTo(expected.getLineFile());
            assertThat(actual.getContingency()).isEqualTo(expected.getContingency());
            assertThat(actual.getConstraint()).isEqualTo(expected.getConstraint());
            assertThat(actual.getBranchToOpen()).isEqualTo(expected.getBranchToOpen());
            assertThat(actual.getBranchToClose()).isEqualTo(expected.getBranchToClose());
            assertThat(actual.getActions()).isEqualTo(expected.getActions());
        }
    }

    @Test
    public void parseEmptyFileTest() {
        String nullString = "";
        List<Remedial> remedials = RemedialReader.parseFile(nullString);
        assertThat(remedials.size()).isEqualTo(0);
    }
}
