import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Button from "react-bootstrap/Button";
import {faArrowCircleLeft} from "@fortawesome/free-solid-svg-icons";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";

type HeaderProps = {
    onBack: () => void;
    disabledBackButton?: boolean;
    title: string;
}

const Header = (props: HeaderProps) => {
    return (
        <>
            <Row>
                <Col>
                    <Button variant="link" onClick={props.onBack} disabled={props.disabledBackButton}>
                        <FontAwesomeIcon icon={faArrowCircleLeft} size="2x" />
                    </Button>
                </Col>
                <Col><h2>{props.title}</h2></Col>
            </Row>
        </>
    )
}

export default Header